#include "Lexer.h"

#include <_ctype.h>
#include <string>

Lexer::Lexer() : lastChar(' '), identifierStr(""), numValue(0){};

int Lexer::nextToken() {
  while (isspace(lastChar))
    lastChar = getchar();

  if (isalpha(lastChar)) {
    identifierStr = lastChar;

    while (isalnum((lastChar = getchar())))
      identifierStr += lastChar;

    if (identifierStr == "function")
      return tok_function;
    if (identifierStr == "return")
      return tok_return;
    if (identifierStr == "if")
      return tok_if;
    if (identifierStr == "else")
      return tok_else;

    return tok_identifier;
  }

  // TODO: it will incorrectly read “1.23.45.67” and handle it as if
  // you typed in “1.23”
  if (isdigit(lastChar) || lastChar == '.') {
    std::string numStr;
    do {
      numStr += lastChar;
      lastChar = getchar();
    } while (isdigit(lastChar) || lastChar == '.');

    numValue = strtod(numStr.c_str(), 0);
  }

  if (lastChar == '#') {
    do {
      lastChar = getchar();
    } while (lastChar != EOF && lastChar != '\n' && lastChar != '\r');

    if (lastChar != EOF) {
      return nextToken();
    }
  }

  if (lastChar == EOF)
    return tok_eof;

  int thisChar = lastChar;
  lastChar = getchar();

  return thisChar;
}
