#include "ast/FunctionAST.h"
#include "PrintVisitor.h"
#include "visitor/Visitor.h"
#include "visitor/ValueVisitor.h"
#include <algorithm>
#include <optional>

FunctionAST::FunctionAST(std::unique_ptr<PrototypeAST> proto,
                         std::optional<std::unique_ptr<ExprAST>> body)
    : proto(std::move(proto)) {

  if (body.has_value())
    this->body = std::move(body.value());
}

void FunctionAST::accept(Visitor &visitor) { visitor.visit(*this); }

llvm::Value* FunctionAST::accept(ValueVisitor &visitor) { return visitor.visit(*this); }
