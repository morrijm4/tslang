#pragma once

namespace llvm {
class Value;
}

class BinaryExprAST;
class CallExprAST;
class FunctionAST;
class NumberExprAST;
class PrototypeAST;
class VariableExprAST;

class ValueVisitor {
public:
  virtual ~ValueVisitor() = default;

  virtual llvm::Value *visit(BinaryExprAST &node) = 0;
  virtual llvm::Value *visit(CallExprAST &node) = 0;
  virtual llvm::Value *visit(VariableExprAST &node) = 0;
  virtual llvm::Value *visit(NumberExprAST &node) = 0;
  virtual llvm::Value *visit(FunctionAST &node) = 0;
  virtual llvm::Value *visit(PrototypeAST &node) = 0;
};
