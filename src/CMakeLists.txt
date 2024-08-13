add_library(compiler
  application/Application.cpp
  ast/BinaryExprAST.cpp
  ast/CallExprAST.cpp
  ast/FunctionAST.cpp
  ast/NumberExprAST.cpp
  ast/PrototypeAST.cpp
  ast/VariableExprAST.cpp
  lexer/Lexer.cpp
  parser/Parser.cpp
  visitor/PrintVisitor.cpp
)

target_include_directories(compiler PUBLIC
  ${CMAKE_SOURCE_DIR}/include/lexer
)

add_executable(emma
  main.cpp
)

target_link_libraries(emma
  compiler
)
