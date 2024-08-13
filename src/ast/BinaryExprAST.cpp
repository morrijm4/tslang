#include "ast/BinaryExprAST.h"
#include "visitor/Visitor.h"

BinaryExprAST::BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
                             std::unique_ptr<ExprAST> RHS)
    : op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

void BinaryExprAST::accept(Visitor &visitor) { visitor.visit(*this); }
