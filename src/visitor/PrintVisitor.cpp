#include "visitor/PrintVisitor.h"
#include "ast/BinaryExprAST.h"
#include "ast/CallExprAST.h"
#include "ast/FunctionAST.h"
#include "ast/NumberExprAST.h"
#include "ast/PrototypeAST.h"
#include "ast/VariableExprAST.h"

#include <functional>
#include <iostream>
#include <string>

PrintVisitor::PrintVisitor() : depth(0), spaceWidth(2) {}

void PrintVisitor::visit(FunctionAST &node) {
  print("FunctionAST");

  nest([this, &node]() {
    node.proto->accept(*this);
    node.body->accept(*this);
  });
}

void PrintVisitor::visit(PrototypeAST &node) {
  print("PrototypeAST");

  nest([this, &node]() {
    print("Name: " + node.name);
    print("Args:");

    nest([this, &node]() {
      for (std::string arg : node.args) {
        print("- " + arg);
      }
    });
  });
}

void PrintVisitor::visit(BinaryExprAST &node) {
  print("BinaryExprAST");

  std::string op = "Op: ";
  op.push_back(node.op);

  nest([this, op, &node]() {
    print(op);
    node.LHS->accept(*this);
    node.RHS->accept(*this);
  });
}

void PrintVisitor::visit(CallExprAST &node) {
  print("CallExprAST");

  nest([this, &node]() {
    print("Callee: " + node.callee);
    print("Args:");

    nest([this, &node]() {
      for (auto it = node.args.begin(); it != node.args.end(); ++it) {
        (*it)->accept(*this);
      }
    });
  });
}

void PrintVisitor::visit(VariableExprAST &node) {
  print("VariableExprAST");

  nest([this, &node]() { print("Name: " + node.name); });
}

void PrintVisitor::visit(NumberExprAST &node) {
  print("NumberExprAST");

  nest([this, &node]() {
    std::string value = std::to_string(node.val);
    print("Value: " + value);
  });
}

void PrintVisitor::print(std::string str) {
  std::string padding(depth * spaceWidth, ' ');

  std::cout << padding << str << std::endl;
}

void PrintVisitor::nest(std::function<void()> func) {
  depth++;
  func();
  depth--;
}
