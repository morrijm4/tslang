#pragma once

#include "visitor/Visitor.h"

#include <functional>
#include <string>
#include <vector>

class PrintVisitor : public Visitor {
private:
  int depth;

  int spaceWidth;

  void print(std::string str);

  void nest(std::function<void()> func);

public:
  PrintVisitor();

  void visit(BinaryExprAST &node) override;

  void visit(CallExprAST &node) override;

  void visit(VariableExprAST &node) override;

  void visit(NumberExprAST &node) override;

  void visit(FunctionAST &node) override;

  void visit(PrototypeAST &node) override;
};
