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
#include "parse/parse.h"


using namespace mocoder;

void Compile(const std::string &source) {
  Lexer lex;
  Lexer::TokenList lst = lex.Lex(source);
  if (!lex.success_) {
    return;
  }
  Parser parse(lst);
  std::shared_ptr<ASTNode> node = parse.Parse();
  if (parse.success_) {
    // node->PrintTree(0);
    node->Eval(nullptr);
  }
}
