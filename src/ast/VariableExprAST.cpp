#include "ast/VariableExprAST.h"
#include "visitor/Visitor.h"
#include "visitor/ValueVisitor.h"

VariableExprAST::VariableExprAST(const std::string &name) : name(name) {}

void VariableExprAST::accept(Visitor &visitor) { visitor.visit(*this); }

llvm::Value* VariableExprAST::accept(ValueVisitor &visitor) { return visitor.visit(*this); }
