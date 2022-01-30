/**
 * @file doloopstmt.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief 
 * @version 0.1
 * @date 2022-01-22
 * 
 * @copyright Copyright (c) 2022 Mocoder Studio
 * 
 */

#ifndef DOLOOPSTMT_H_
#define DOLOOPSTMT_H_

#include "ast/astnode.h"
#include "lex/lexer.h"
#include "ast/expression/condexpr.h"
#include <list>

namespace mocoder {
class DoLoopStmt : public ASTNode {
public:
  Condexpr *cond_;
  std::list<ASTNode*> stmts_;
  std::unordered_set<std::string> declvars_;
  DoLoopStmt(Condexpr* cond,std::list<ASTNode*>& stmts):cond_(cond),stmts_(std::move(stmts)){
  }
  virtual ~DoLoopStmt() {
    delete cond_;
    for(ASTNode *stmt:stmts_) {
      delete stmt;
    }
  }
  virtual void PrintTree(int depth) override {
    for(int i=0;i<depth;++i) {
      std::cout<<" ";
    }
    std::cout<<depth<<":"<<"DoLoopStmt(declvars=";
    for(const std::string& var:declvars_) {
      std::cout<<var<<",";
    }
    std::cout<<")";
    std::cout<<std::endl;
    cond_->PrintTree(depth+1);
    for(ASTNode* stmt:stmts_) {
      stmt->PrintTree(depth+1);
    }
  }
  virtual std::string GenJS() override {
    std::string result;
    #ifdef DEBUG_MODE
    result+="do{\n";
    #else
    result+="do{";
    #endif
    for(std::string varname:declvars_) {
      #ifdef DEBUG_MODE
      result+="var "+varname+"=0.0;\n";
      #else
      result+="var "+varname+"=0.0;";
      #endif
    }
    for(ASTNode* stmt:stmts_) {
      result+=stmt->GenJS();
    }
    result+="}while(";
    result+=cond_->GenJS();
    #ifdef DEBUG_MODE
    result+=");\n";
    #else
    result+=");";
    #endif
    return result;
  }

};
} // namespace mocoder

#endif
