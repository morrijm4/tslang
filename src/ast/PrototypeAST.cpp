#include "ast/PrototypeAST.h"
#include "visitor/Visitor.h"
#include "visitor/ValueVisitor.h"

PrototypeAST::PrototypeAST(const std::string &name,
                           std::vector<std::string> args)
    : name(name), args(std::move(args)) {}

void PrototypeAST::accept(Visitor &visitor) { visitor.visit(*this); }

llvm::Value* PrototypeAST::accept(ValueVisitor &visitor) { return visitor.visit(*this); }

const std::string &PrototypeAST::getName() { return this->name; }

