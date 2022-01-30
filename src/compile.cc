/**
 * @file compile.cc
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "compile.h"
#include "lex/lexer.h"
#include "parse/parse.h"
#include <iostream>

using namespace mocoder;

std::string Compile(const std::string &source) {
  //std::cout<<source<<std::endl;
  Lexer lex;
  Lexer::TokenList lst=lex.Lex(source);
  Parser parse(lst);
  std::shared_ptr<ASTNode> node=parse.Parse();
  //node->PrintTree(0);
  return node->GenJS();
}
