#include "ast/VariableExprAST.h"
#include "visitor/Visitor.h"

VariableExprAST::VariableExprAST(const std::string &name) : name(name) {}

void VariableExprAST::accept(Visitor &visitor) { visitor.visit(*this); }
