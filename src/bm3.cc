/**
 * @file bm3.cc
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-01-16
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#include "compile.h"
#include "lex/lexer.h"
#include "parse/parse.h"
#include "vm/vm.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace mocoder;
using namespace std;

int main(int argc, char **argv) {
  if (argc != 3) {
    return 233;
  }
  std::ifstream source(argv[2]);
  std::stringstream buf;
  buf << source.rdbuf();
  std::string sourcecode(buf.str());
  auto t = Compile(sourcecode);
  if (!strcmp(argv[1], "tree")) {
    t->PrintTree(0);
  } else if (!strcmp(argv[1], "js")) {
    t->GenJS();
  } else if (!strcmp(argv[1], "run")) {
    t->Eval(nullptr);
  } else if (!strcmp(argv[1], "vm")) {
    vector<Op> main;
    map<string,vector<Op>> func;
    t->GenRootVM(main, func);
    VM vm;
    vm.main_ = main;
    vm.funcs_ = func;
    vm.Run();
  } else {
    return 114;
  }
  return 0;
}
