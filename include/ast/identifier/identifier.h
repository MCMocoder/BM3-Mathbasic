/**
 * @file identifier.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef IDENTIFIER_H_
#define IDENTIFIER_H_

#include "ast/astnode.h"
#include "lex/lexer.h"
#include <string>

namespace mocoder {
class Identifier : public ASTNode {
public:
  std::string name_;
  Identifier(const std::string& name):name_(name){}
  virtual void PrintTree(int depth) override {
    for(int i=0;i<depth;++i) {
      std::cout<<" ";
    }
    std::cout<<depth<<":"<<"Identifier(name="<<name_<<")";
    std::cout<<std::endl;
  }
  virtual std::string GenJS() override {
    return name_;
  }
};
} // namespace mocoder

#endif


