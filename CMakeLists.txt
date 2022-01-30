cmake_minimum_required(VERSION 3.0.0)
project(bm3 VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} src/bm3.cc
                               src/lex/lexer.cc
                               src/parse/parse.cc
                               src/compile.cc
)

include_directories("./include")
include_directories("/usr/local/Cellar/emscripten/3.1.0/libexec/system/include")
 
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
