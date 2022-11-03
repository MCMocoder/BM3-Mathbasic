cmake_minimum_required(VERSION 3.0.0)
project(bm3 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

include(CTest)
enable_testing()

add_executable(bm3 src/bm3.cc
                   src/lex/lexer.cc
                   src/parse/parse.cc
                   src/compile.cc
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
