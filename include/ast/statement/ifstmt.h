/**
 * @file ifstmt.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief 
 * @version 0.1
 * @date 2022-01-22
 * 
 * @copyright Copyright (c) 2022 Mocoder Studio
 * 
 */

#ifndef IFSTMT_H_
#define IFSTMT_H_

#include "ast/astnode.h"
#include "ast/statement/elsestmt.h"
#include "lex/lexer.h"
#include "ast/expression/condexpr.h"
#include <list>
#include <string>
#include <unordered_set>

namespace mocoder {
class IfStmt : public ASTNode {
public:
  Ptr<Condexpr> cond_;
  std::list<Ptr<ASTNode> > stmts_;
  Ptr<ElseStmt> else_;
  std::unordered_set<std::string> declvars_;
  IfStmt(Ptr<Condexpr> cond,std::list<Ptr<ASTNode> > &stmts):cond_(cond),stmts_(stmts){
    else_=nullptr;
  }
  virtual void PrintTree(int depth) override {
    for(int i=0;i<depth;++i) {
      std::cout<<" ";
    }
    std::cout<<depth<<":"<<"IfStmt(declvars=";
    for(const std::string& var:declvars_) {
      std::cout<<var<<",";
    }
    std::cout<<")";
    std::cout<<std::endl;
    cond_->PrintTree(depth+1);
    for(Ptr<ASTNode>  stmt:stmts_) {
      stmt->PrintTree(depth+1);
    }
  }
  virtual std::string GenJS() override {
    std::string result;
    result+="if(";
    result+=cond_->GenJS();
    #ifdef DEBUG_MODE
    result+="){\n";
    #else
    result+="){";
    #endif
    for(std::string varname:declvars_) {
      #ifdef DEBUG_MODE
      result+="var "+varname+"=0.0;\n";
      #else
      result+="var "+varname+"=0.0;";
      #endif
    }
    for(Ptr<ASTNode>  stmt:stmts_) {
      result+=stmt->GenJS();
    }
    if(else_!=nullptr) {
      result+="}";
      result+=else_->GenJS();
    } else {
      #ifdef DEBUG_MODE
      result+="}\n";
      #else
      result+="}";
      #endif
    }
    return result;
  }
};
} // namespace mocoder

#endif

