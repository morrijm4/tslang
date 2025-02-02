#pragma once

#include "ExprAST.h"
#include "PrototypeAST.h"
#include <optional>

class FunctionAST : public BaseAST {
public:
  std::unique_ptr<PrototypeAST> proto;
  std::optional<std::unique_ptr<ExprAST>> body;

  FunctionAST(std::unique_ptr<PrototypeAST> proto,
              std::optional<std::unique_ptr<ExprAST>> body);

  void accept(Visitor &visitor) override;

  llvm::Value *accept(ValueVisitor &visitor) override;
};
