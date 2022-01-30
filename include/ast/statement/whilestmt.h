/**
 * @file whilestmt.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief 
 * @version 0.1
 * @date 2022-01-22
 * 
 * @copyright Copyright (c) 2022 Mocoder Studio
 * 
 */

#ifndef WHILESTMT_H_
#define WHILESTMT_H_

#include "ast/astnode.h"
#include "lex/lexer.h"
#include "ast/expression/condexpr.h"
#include <list>

namespace mocoder {
class WhileStmt : public ASTNode {
public:
  Condexpr* cond_;
  std::list<ASTNode*> stmts_;
  std::unordered_set<std::string> declvars_;
  WhileStmt(Condexpr* cond,std::list<ASTNode*> &stmts):cond_(cond),stmts_(stmts) {
  }
  virtual ~WhileStmt() {
    delete cond_;
    for(ASTNode* expr:stmts_) {
      delete expr;
    }
  }
  virtual void PrintTree(int depth) override {
    for(int i=0;i<depth;++i) {
      std::cout<<" ";
    }
    std::cout<<depth<<":"<<"WhileStmt(declvars=";
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
    for(ASTNode* stmt:stmts_) {
      result+=stmt->GenJS();
    }
    #ifdef DEBUG_MODE
    result+="}\n";
    #else
    result+="}";
    #endif
    return result;
  }
};
} // namespace mocoder

#endif

