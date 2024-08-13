#pragma once

#include "ast/ExprAST.h"
#include "ast/FunctionAST.h"
#include "ast/PrototypeAST.h"
#include "lexer/Lexer.h"

#include <memory>
#include <map>

class Parser {
private:
  Lexer lexer;

  std::map<char, int> binOpPrecedence;

public:
  int currTok;

  Parser();

  int getCurrentToken();

  int getNextToken();

  int getTokenPrecedence();

  std::unique_ptr<ExprAST> parseNumberExpr();

  std::unique_ptr<ExprAST> parseParenExpr();

  std::unique_ptr<ExprAST> parseIdentifierExpr();

  std::unique_ptr<ExprAST> parsePrimaryExpr();

  std::unique_ptr<ExprAST> parseExpression();

  std::unique_ptr<ExprAST> parseBinOpRHS(int ExprPrec,
                                         std::unique_ptr<ExprAST> LHS);

  std::unique_ptr<PrototypeAST> parsePrototype();

  std::unique_ptr<FunctionAST> parseDefinition();

  std::unique_ptr<FunctionAST> parseTopLevelExpr();
};
