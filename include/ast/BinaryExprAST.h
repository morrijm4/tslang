#pragma once

#include "ExprAST.h"

#include <memory>

class BinaryExprAST : public ExprAST {
public:
  char op;

  std::unique_ptr<ExprAST> LHS, RHS;

  BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS);

  void accept(Visitor &visitor) override;
  llvm::Value *accept(ValueVisitor &visitor) override;
};
