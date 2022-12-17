/**
 * @file compile.cc
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-01-30
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#include "compile.h"

#include <iostream>

#include "lex/lexer.h"
#include "vm/vm.h"


using namespace mocoder;

std::shared_ptr<RootNode> Compile(const std::string &source) {
  Lexer lex;
  Lexer::TokenList lst = lex.Lex(source);
  if (!lex.success_) {
    return nullptr;
  }
  Parser parse(lst);
  std::shared_ptr<RootNode> node = parse.Parse();
  return parse.success_?node:nullptr;
}
