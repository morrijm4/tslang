#include "app/App.h"
#include "Lexer.h"
#include "parser/Parser.h"
#include "visitor/PrintVisitor.h"

App::App() : parser(Parser()) {}

void App::run() {
  std::unique_ptr<FunctionAST> node;
  PrintVisitor printVisitor;

  parser.getNextToken();

  while (true) {
    switch (parser.currTok) {
    case tok_eof:
      return;
    case ';':
      parser.getNextToken();
      break;
    case tok_function:
      node = parser.parseDefinition();
      if (node) node->accept(printVisitor);
      break;
    default:
      node = parser.parseTopLevelExpr();
      if (node) node->accept(printVisitor);
      break;
    }

  }
}
