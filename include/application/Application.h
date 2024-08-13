#pragma once

#include "parser/Parser.h"

class Application {

private:
  Parser parser;

public:
  Application();

  void run();
};
