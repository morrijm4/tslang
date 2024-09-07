#pragma once

namespace llvm {
class Value;
}

class Visitor;
class ValueVisitor;

class BaseAST {
public:
  virtual ~BaseAST() = default;

  virtual void accept(Visitor &visitor) = 0;
  virtual llvm::Value *accept(ValueVisitor &visitor) = 0;
};
