#include "ast/NumberExprAST.h"
#include "visitor/Visitor.h"
#include "visitor/ValueVisitor.h"

NumberExprAST::NumberExprAST(double val) : val(val) {}

void NumberExprAST::accept(Visitor &visitor) { visitor.visit(*this); }

llvm::Value* NumberExprAST::accept(ValueVisitor &visitor) { return visitor.visit(*this); }
