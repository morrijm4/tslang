add_library(app
  app/App.cpp
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

target_include_directories(app PUBLIC
  ${CMAKE_SOURCE_DIR}/include/lexer
  ${CMAKE_SOURCE_DIR}/include/app
  ${CMAKE_SOURCE_DIR}/include/ast
  ${CMAKE_SOURCE_DIR}/include/parser
  ${CMAKE_SOURCE_DIR}/include/visitor
)

add_executable(tslang
  main.cpp
)

target_link_libraries(tslang
  app
)
