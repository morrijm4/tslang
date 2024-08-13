#include "ast/CallExprAST.h"
#include "visitor/Visitor.h"

CallExprAST::CallExprAST(const std::string &callee,
                         std::vector<std::unique_ptr<ExprAST>> args)
    : callee(callee), args(std::move(args)) {}

void CallExprAST::accept(Visitor &visitor) { visitor.visit(*this); }
