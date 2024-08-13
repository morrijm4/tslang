#pragma once

class BinaryExprAST;
class CallExprAST;
class FunctionAST;
class NumberExprAST;
class PrototypeAST;
class VariableExprAST;

class Visitor {
public:
  virtual ~Visitor() = default;

  virtual void visit(BinaryExprAST &node) {}

  virtual void visit(CallExprAST &node){};

  virtual void visit(VariableExprAST &node){};

  virtual void visit(NumberExprAST &node){};

  virtual void visit(FunctionAST &node){};

  virtual void visit(PrototypeAST &node){};
};
