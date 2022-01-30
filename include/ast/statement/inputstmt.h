/**
 * @file inputstmt.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef INPUTSTMT_H_
#define INPUTSTMT_H_

#include "ast/astnode.h"
#include "lex/lexer.h"
#include "ast/identifier/identifierlist.h"

namespace mocoder {
class InputStmt : public ASTNode {
public:
  IdentifierList* vars_;
  InputStmt(IdentifierList* vars):vars_(vars){}
  virtual ~InputStmt() {
    delete vars_;
  }
  virtual void PrintTree(int depth) override {
    for(int i=0;i<depth;++i) {
      std::cout<<" ";
    }
    std::cout<<depth<<":"<<"InputStmt";
    std::cout<<std::endl;
    vars_->PrintTree(depth+1);
  }
  virtual std::string GenJS() override {
    std::string result;
    for(ASTNode* node:vars_->ids_) {
      result+=node->GenJS();
      result+="=outside.input();\n";
    }
    return result;
  }
};
} // namespace mocoder

#endif
