#pragma once

#include "visitor/ValueVisitor.h"
#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/PassInstrumentation.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/StandardInstrumentations.h"

#include <map>
#include <memory>

class CodegenVisitor : public ValueVisitor {
private:
  std::unique_ptr<llvm::LLVMContext> context;
  std::unique_ptr<llvm::Module> module;
  std::unique_ptr<llvm::IRBuilder<>> builder;

  // Optimization Managers
  std::unique_ptr<llvm::FunctionPassManager> functionPM;
  std::unique_ptr<llvm::LoopAnalysisManager> loopAM;
  std::unique_ptr<llvm::FunctionAnalysisManager> functionAM;
  std::unique_ptr<llvm::CGSCCAnalysisManager> callGraphAM;
  std::unique_ptr<llvm::ModuleAnalysisManager> moduleAM;
  std::unique_ptr<llvm::PassInstrumentationCallbacks> passInstrumentationCallbacks;
  std::unique_ptr<llvm::StandardInstrumentations> standardInstrumentations;

  std::map<std::string, llvm::Value *> namedValues;

public:
  CodegenVisitor();

  void print();

  llvm::Value *visit(BinaryExprAST &node) override;
  llvm::Value *visit(CallExprAST &node) override;
  llvm::Value *visit(VariableExprAST &node) override;
  llvm::Value *visit(NumberExprAST &node) override;
  llvm::Value *visit(FunctionAST &node) override;
  llvm::Value *visit(PrototypeAST &node) override;
};
