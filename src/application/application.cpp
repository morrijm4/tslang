#include "application/Application.h"
#include "Lexer.h"
#include "parser/Parser.h"
#include "visitor/PrintVisitor.h"

Application::Application() : parser(Parser()) {}

void Application::run() {
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
      break;
    default:
      node = parser.parseTopLevelExpr();
      break;
    }

    node->accept(printVisitor);
  }
}
