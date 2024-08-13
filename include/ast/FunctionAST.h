#pragma once

#include "ExprAST.h"
#include "PrototypeAST.h"

class FunctionAST : public BaseAST {
public:
  std::unique_ptr<PrototypeAST> proto;
  std::unique_ptr<ExprAST> body;

  FunctionAST(std::unique_ptr<PrototypeAST> proto,
              std::unique_ptr<ExprAST> body);

  void accept(Visitor &visitor) override;
};
