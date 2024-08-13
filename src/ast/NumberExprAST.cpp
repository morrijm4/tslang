#include "ast/NumberExprAST.h"
#include "visitor/Visitor.h"

NumberExprAST::NumberExprAST(double val) : val(val) {}

void NumberExprAST::accept(Visitor &visitor) { visitor.visit(*this); }
