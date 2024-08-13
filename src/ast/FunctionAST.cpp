#include "ast/FunctionAST.h"
#include "visitor/Visitor.h"

FunctionAST::FunctionAST(std::unique_ptr<PrototypeAST> proto,
                         std::unique_ptr<ExprAST> body)
    : proto(std::move(proto)), body(std::move(body)) {}

void FunctionAST::accept(Visitor &visitor) { visitor.visit(*this); }
