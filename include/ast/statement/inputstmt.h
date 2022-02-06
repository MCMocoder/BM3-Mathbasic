/**
 * @file inputstmt.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief 
 * @version 0.1
 * @date 2022-01-22
 * 
 * @copyright Copyright (c) 2022 Mocoder Studio
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
  Ptr<IdentifierList> vars_;
  InputStmt(Ptr<IdentifierList> vars):vars_(vars){}

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
    for(Ptr<ASTNode> node:vars_->ids_) {
      result+=node->GenJS();
      #ifdef DEBUG_MODE
      result+="=outside.input();\n";
      #else
      result+="=outside.input();";
      #endif
    }
    return result;
  }
};
} // namespace mocoder

#endif
