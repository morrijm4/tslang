#pragma once

#include "ExprAST.h"

#include <string>
#include <vector>

class PrototypeAST : public BaseAST {
private:

public:
  std::string name;
  std::vector<std::string> args;

  PrototypeAST(const std::string &name, std::vector<std::string> args);

  void accept(Visitor &visitor) override;

  llvm::Value *accept(ValueVisitor &visitor) override;

  const std::string &getName();
};
