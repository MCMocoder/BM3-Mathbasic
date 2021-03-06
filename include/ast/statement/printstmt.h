/**
 * @file printstmt.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief 
 * @version 0.1
 * @date 2022-01-22
 * 
 * @copyright Copyright (c) 2022 Mocoder Studio
 * 
 */

#ifndef PRINTSTMT_H_
#define PRINTSTMT_H_

#include "ast/astnode.h"
#include "lex/lexer.h"
#include "ast/expression/valexprlist.h"
#include <string>

namespace mocoder {
class PrintStmt : public ASTNode {
public:
  Ptr<ValexprList> exprs_;
  PrintStmt(Ptr<ValexprList> exprs):exprs_(exprs){}
  
  virtual void PrintTree(int depth) override {
    for(int i=0;i<depth;++i) {
      std::cout<<" ";
    }
    std::cout<<depth<<":"<<"PrintStmt";
    std::cout<<std::endl;
    exprs_->PrintTree(depth+1);
  }
  virtual std::string GenJS() override {
    std::string result;
    for(Ptr<ASTNode> node:exprs_->exprs_) {
      result+="outside.print(";
      result+=node->GenJS();
      #ifdef DEBUG_MODE
      result+=");\n";
      #else
      result+=");";
      #endif
    }
    return result;
  }
};
} // namespace mocoder

#endif
