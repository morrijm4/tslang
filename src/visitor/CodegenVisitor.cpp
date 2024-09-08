#include "ast/BinaryExprAST.h"
#include "ast/CallExprAST.h"
#include "ast/FunctionAST.h"
#include "ast/NumberExprAST.h"
#include "ast/PrototypeAST.h"
#include "ast/VariableExprAST.h"
#include "visitor/CodegenVisiter.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassInstrumentation.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"

#include <cstdio>
#include <memory>
#include <string>
#include <system_error>
#include <vector>

CodegenVisitor::CodegenVisitor()
    : context(std::make_unique<llvm::LLVMContext>()),
      builder(std::make_unique<llvm::IRBuilder<>>(*context)),
      module(std::make_unique<llvm::Module>("tslang", *context)),
      functionPM(std::make_unique<llvm::FunctionPassManager>()),
      loopAM(std::make_unique<llvm::LoopAnalysisManager>()),
      functionAM(std::make_unique<llvm::FunctionAnalysisManager>()),
      callGraphAM(std::make_unique<llvm::CGSCCAnalysisManager>()),
      moduleAM(std::make_unique<llvm::ModuleAnalysisManager>()),
      passInstrumentationCallbacks(
          std::make_unique<llvm::PassInstrumentationCallbacks>()),
      standardInstrumentations(std::make_unique<llvm::StandardInstrumentations>(
          *context, /* debug logging */ true))

{
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  std::string targetTriple = llvm::sys::getDefaultTargetTriple();

  std::string error;
  const llvm::Target *target =
      llvm::TargetRegistry::lookupTarget(targetTriple, error);

  if (!target) {
    llvm::errs() << error;
  }

  std::string cpu = "generic";
  std::string features = "";
  llvm::TargetOptions opt;

  this->targetMachine = target->createTargetMachine(targetTriple, cpu, features,
                                                    opt, llvm::Reloc::PIC_);

  this->module->setDataLayout(this->targetMachine->createDataLayout());
  this->module->setTargetTriple(targetTriple);

  standardInstrumentations->registerCallbacks(
      *this->passInstrumentationCallbacks, this->moduleAM.get());

  // Do simple "peephole" optimizations and bit-twiddling optzns.
  this->functionPM->addPass(llvm::InstCombinePass());
  // Reassociate expressions.
  this->functionPM->addPass(llvm::ReassociatePass());
  // Eliminate Common SubExpressions.
  this->functionPM->addPass(llvm::GVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  this->functionPM->addPass(llvm::SimplifyCFGPass());

  llvm::PassBuilder passBuilder;
  passBuilder.registerModuleAnalyses(*this->moduleAM);
  passBuilder.registerFunctionAnalyses(*this->functionAM);
  passBuilder.crossRegisterProxies(*this->loopAM, *this->functionAM,
                                   *this->callGraphAM, *this->moduleAM);
}

void CodegenVisitor::print() { this->module->print(llvm::errs(), nullptr); }

int CodegenVisitor::emitObjectFile() {

  std::string filename = "output.o";
  std::error_code errorCode;
  llvm::raw_fd_ostream dest(filename, errorCode, llvm::sys::fs::OF_None);

  if (errorCode) {
    llvm::errs() << "Could not open file: " << errorCode.message();
    return 1;
  }

  llvm::legacy::PassManager pass;
  llvm::CodeGenFileType fileType = llvm::CodeGenFileType::ObjectFile;

  if (this->targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType)) {
    llvm::errs() << "TargetMachine can't emit a file of this type";
    return 1;
  }

  pass.run(*this->module);
  dest.flush();

  return 0;
}

llvm::Value *CodegenVisitor::visit(BinaryExprAST &node) {
  llvm::Value *L = node.LHS->accept(*this);
  llvm::Value *R = node.RHS->accept(*this);

  if (!L || !R)
    return nullptr;

  switch (node.op) {
  case '+':
    return this->builder->CreateFAdd(L, R, "addtmp");
  case '-':
    return this->builder->CreateFSub(L, R, "subtmp");
  case '*':
    return this->builder->CreateFMul(L, R, "multmp");
  case '<':
    L = this->builder->CreateFCmpULT(L, R, "cmptmp");
    // Convert bool 0/1 to double 0.0 or 1.0
    return this->builder->CreateUIToFP(
        L, llvm::Type::getDoubleTy(*this->context), "booltmp");
  default:
    fprintf(stderr, "Invalid binary operator\n");
    return nullptr;
  }
};

llvm::Value *CodegenVisitor::visit(CallExprAST &node) {
  llvm::Function *calleeFunction = this->module->getFunction(node.callee);
  if (!calleeFunction) {
    fprintf(stderr, "Unknown function referenced\n");
    return nullptr;
  }

  if (calleeFunction->arg_size() != node.args.size()) {
    fprintf(stderr, "Incorrect # arguments passed\n");
    return nullptr;
  }

  std::vector<llvm::Value *> argsV;
  for (unsigned i = 0, end = node.args.size(); i != end; ++i) {
    argsV.push_back(node.args[i]->accept(*this));
    if (!argsV.back())
      return nullptr;
  }

  return this->builder->CreateCall(calleeFunction, argsV, "calltmp");
};

llvm::Value *CodegenVisitor::visit(VariableExprAST &node) {
  llvm::Value *v = this->namedValues[node.name];
  if (!v)
    fprintf(stderr, "Unknown variable name: %s\n", node.name.c_str());
  return v;
};

llvm::Value *CodegenVisitor::visit(NumberExprAST &node) {
  return llvm::ConstantFP::get(*this->context, llvm::APFloat(node.val));
};

/*
 * This code does have a bug, though: If the FunctionAST::codegen() method finds
 * an existing IR Function, it does not validate its signature against the
 * definition’s own prototype. This means that an earlier ‘extern’ declaration
 * will take precedence over the function definition’s signature, which can
 * cause codegen to fail, for instance if the function arguments are named
 * differently. There are a number of ways to fix this bug, see what you can
 * come up with! Here is a testcase:
 *
 * ```
 * extern foo(a);     # ok, defines foo.
 * def foo(b) b;      # Error: Unknown variable name. (decl using 'a' takes
 * precedence).
 * ```
 */
llvm::Value *CodegenVisitor::visit(FunctionAST &node) {
  if (!node.body.has_value())
    return nullptr;

  llvm::Function *func = this->module->getFunction(node.proto->getName());

  if (!func)
    func = static_cast<llvm::Function *>(node.proto->accept(*this));

  if (!func)
    return nullptr;

  llvm::BasicBlock *basicBlock =
      llvm::BasicBlock::Create(*this->context, "entry", func);
  this->builder->SetInsertPoint(basicBlock);

  this->namedValues.clear();
  for (llvm::Argument &arg : func->args())
    this->namedValues[std::string(arg.getName())] = &arg;

  if (llvm::Value *returnValue = node.body.value()->accept(*this)) {
    this->builder->CreateRet(returnValue);

    llvm::verifyFunction(*func);

    this->functionPM->run(*func, *this->functionAM);

    return func;
  }

  func->eraseFromParent();
  return nullptr;
};

llvm::Value *CodegenVisitor::visit(PrototypeAST &node) {

  std::vector<llvm::Type *> doubleTypes(
      node.args.size(), llvm::Type::getDoubleTy(*this->context));

  llvm::FunctionType *funcType = llvm::FunctionType::get(
      llvm::Type::getDoubleTy(*this->context), doubleTypes, false);

  llvm::Function *func = llvm::Function::Create(
      funcType, llvm::Function::ExternalLinkage, node.name, this->module.get());

  unsigned i = 0;
  for (llvm::Argument &arg : func->args())
    arg.setName(node.args[i++]);

  return func;
};
