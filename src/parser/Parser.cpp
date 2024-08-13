#include "parser/Parser.h"
#include "ast/BinaryExprAST.h"
#include "ast/CallExprAST.h"
#include "ast/NumberExprAST.h"
#include "ast/VariableExprAST.h"

Parser::Parser() : lexer(Lexer()) {
  // multiplicative operators
  binOpPrecedence['*'] = 120;
  binOpPrecedence['/'] = 120;
  binOpPrecedence['%'] = 120;

  // additive operators
  binOpPrecedence['+'] = 110;
  binOpPrecedence['-'] = 110;

  // relational operators
  binOpPrecedence['<'] = 90;
  binOpPrecedence['>'] = 90;
}

int Parser::getNextToken() { return currTok = lexer.nextToken(); }

int Parser::getTokenPrecedence() {
  if (!isascii(currTok))
    return -1;

  int TokPrec = binOpPrecedence[currTok];
  if (TokPrec <= 0)
    return -1;

  return TokPrec;
}

std::unique_ptr<ExprAST> Parser::parseNumberExpr() {
  auto Result = std::make_unique<NumberExprAST>(lexer.numValue);
  getNextToken();
  return std::move(Result);
}

std::unique_ptr<ExprAST> Parser::parseParenExpr() {
  getNextToken(); // eat '('
  auto V = parseExpression();
  if (!V)
    return nullptr;

  if (currTok != ')') {
    fprintf(stderr, "expected ')' recieved '%c\n'", currTok);
    return nullptr;
  }

  getNextToken(); // eat ')'
  return V;
}

std::unique_ptr<ExprAST> Parser::parseIdentifierExpr() {
  std::string IdName = lexer.identifierStr;

  getNextToken(); // eat identifier

  if (currTok != '(')
    return std::make_unique<VariableExprAST>(IdName);

  // function call
  getNextToken();

  std::vector<std::unique_ptr<ExprAST>> Args;
  if (currTok != ')') {
    while (true) {
      if (auto Arg = parseExpression())
        Args.push_back(std::move(Arg));
      else
        return nullptr; // TODO: understand why this is necessary

      if (currTok == ')')
        break;

      if (currTok != ',') {
        fprintf(stderr, "expected ')' or ',' in argument list");
        return nullptr;
      }
      getNextToken();
    }
  }

  getNextToken(); // eat ')'

  return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

std::unique_ptr<ExprAST> Parser::parsePrimaryExpr() {
  switch (currTok) {
  default:
    fprintf(stderr, "unknown token when expecting an expression: %c\n",
            currTok);
    return nullptr;
  case tok_if:
  case tok_else:
  case tok_return:
  case tok_identifier:
    return parseIdentifierExpr();
  case tok_number:
    return parseNumberExpr();
  case '(':
    return parseParenExpr();
  }
}

std::unique_ptr<ExprAST> Parser::parseExpression() {
  auto LHS = parsePrimaryExpr();
  if (!LHS)
    return nullptr;

  return parseBinOpRHS(0, std::move(LHS));
}

std::unique_ptr<ExprAST> Parser::parseBinOpRHS(int ExprPrec,
                                               std::unique_ptr<ExprAST> LHS) {
  while (true) {
    int TokPrec = getTokenPrecedence();

    if (TokPrec < ExprPrec)
      return LHS;

    int BinOp = currTok;
    getNextToken();

    auto RHS = parsePrimaryExpr();
    if (!RHS)
      return nullptr;

    int NextPrec = getTokenPrecedence();
    if (TokPrec < NextPrec) {
      RHS = parseBinOpRHS(TokPrec + 1, std::move(RHS));
      if (!RHS)
        return nullptr;
    }

    LHS =
        std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
  }
}

std::unique_ptr<PrototypeAST> Parser::parsePrototype() {
  if (currTok != tok_identifier) {
    fprintf(stderr, "expected function name in prototype recieved %c\n",
            currTok);
    return nullptr;
  }

  std::string FnName = lexer.identifierStr;
  getNextToken();

  if (currTok != '(') {
    fprintf(stderr, "expected '(' in prototype recieved %c\n", currTok);
    return nullptr;
  }

  std::vector<std::string> ArgNames;
  do {
    getNextToken();

    if (currTok == tok_identifier)
      ArgNames.push_back(lexer.identifierStr);
    else if (currTok == ',')
      getNextToken(); // eat ','
  } while (currTok == tok_identifier);

  if (currTok != ')') {
    fprintf(stderr, "expected ')' in prototype recieved %c\n", currTok);
    return nullptr;
  }

  getNextToken(); // eat ')'

  return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

std::unique_ptr<FunctionAST> Parser::parseDefinition() {
  getNextToken(); // eat 'function'

  auto Proto = parsePrototype();
  if (!Proto)
    return nullptr;

  if (currTok != '{') {
    fprintf(stderr, "expected '{' for function definition recieved %c\n",
            currTok);
    return nullptr;
  }
  getNextToken(); // eat '{'

  auto Expr = parseExpression();
  if (!Expr)
    return nullptr;

  if (currTok != '}') {
    fprintf(stderr, "expected '}' for function definition recieved %c\n",
            currTok);
    return nullptr;
  }
  getNextToken(); // eat '}'

  return std::make_unique<FunctionAST>(std::move(Proto), std::move(Expr));
}

std::unique_ptr<FunctionAST> Parser::parseTopLevelExpr() {
  auto Expr = parseExpression();
  if (!Expr)
    return nullptr;

  auto Proto = std::make_unique<PrototypeAST>("", std::vector<std::string>());
  return std::make_unique<FunctionAST>(std::move(Proto), std::move(Expr));
}
