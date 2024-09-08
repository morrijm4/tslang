#pragma once

#include "parser/Parser.h"

class App {

private:
  Parser parser;

public:
  App();

  int run();
};
