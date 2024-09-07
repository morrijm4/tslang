#pragma once

#include "ExprAST.h"

class NumberExprAST : public ExprAST {
public:
  double val;

  NumberExprAST(double val);

  void accept(Visitor &visitor) override;
  llvm::Value *accept(ValueVisitor &visitor) override;
};
