#include "app/App.h"
#include "Lexer.h"
#include "parser/Parser.h"
#include "visitor/PrintVisitor.h"
#include "visitor/CodegenVisiter.h"

#include <iostream>

App::App() : parser(Parser()) {}

void App::run() {
  std::unique_ptr<FunctionAST> node;
  PrintVisitor printVisitor;
  CodegenVisitor codegenVisitor;

  parser.getNextToken();

  while (true) {
    switch (parser.currTok) {
    case tok_eof:
      std::cout << "\n\n\n";
      codegenVisitor.print();
      return;
    case ';':
      parser.getNextToken();
      break;
    case tok_function:
      node = parser.parseDefinition();
      if (node) {
	node->accept(printVisitor);
	node->accept(codegenVisitor);
      }
      break;
    default:
      node = parser.parseTopLevelExpr();
      if (node) {
	node->accept(printVisitor);
	node->accept(codegenVisitor);
      }
      break;
    }
  }
}
