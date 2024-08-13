#pragma once

class Visitor;

class BaseAST {
public:
  virtual ~BaseAST() = default;

  virtual void accept(Visitor &visitor) = 0;
};
