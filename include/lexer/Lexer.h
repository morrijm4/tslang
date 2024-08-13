#pragma once

#include <string>

enum Token {
  tok_eof = -1,

  // commands
  tok_function = -2,
  tok_return = -3,
  tok_if = -4,
  tok_else = -5,

  // primary
  tok_identifier = -6,
  tok_number = -7,
};

class Lexer {
public:
  int lastChar;
  std::string identifierStr;
  double numValue;

  Lexer();

  int nextToken();
};
