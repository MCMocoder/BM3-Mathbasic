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

#include <fstream>
#include <sstream>
#include <string>
#include "lex/lexer.h"
#include "parse/parse.h"
#include "compile.h"


using namespace mocoder;
using namespace std;

int main(int argc,char** argv) {
  if(argc!=2) {
    return 233;
  }
  std::ifstream source(argv[1]);
  std::stringstream buf;
  buf<<source.rdbuf();
  std::string sourcecode(buf.str());
  cout<<Compile(sourcecode);
  return 0;
}
