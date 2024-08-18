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

set(LLVM_DIR "/opt/homebrew//Cellar/llvm/18.1.8/lib/cmake/llvm")
find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(tslang
  main.cpp
)

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(tslang
  app
  ${llvm_libs}
)
