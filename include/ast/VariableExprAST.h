#pragma once

#include "ExprAST.h"

#include <string>

class VariableExprAST : public ExprAST {
public:
  std::string name;

  VariableExprAST(const std::string &name);

  void accept(Visitor &visitor) override;
};
