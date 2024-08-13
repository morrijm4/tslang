#pragma once

#include "ExprAST.h"

#include <string>
#include <vector>

class CallExprAST : public ExprAST {

public:
  std::string callee;

  std::vector<std::unique_ptr<ExprAST>> args;

  CallExprAST(const std::string &callee,
              std::vector<std::unique_ptr<ExprAST>> args);

  void accept(Visitor &visitor) override;
};
