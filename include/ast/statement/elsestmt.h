/**
 * @file elsestmt.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-01-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef ELSESTMT_H_
#define ELSESTMT_H_

#include "ast/astnode.h"
#include "lex/lexer.h"
#include "ast/expression/condexpr.h"
#include <list>
#include <string>
#include <unordered_set>

namespace mocoder {
class ElseStmt : public ASTNode {
public:
  std::list<ASTNode*> stmts_;
  std::unordered_set<std::string> declvars_;
  ElseStmt(std::list<ASTNode*> &stmts):stmts_(std::move(stmts)){
  }
  virtual ~ElseStmt() {
    for(ASTNode* stmt:stmts_) {
      delete stmt;
    }
  }
  virtual void PrintTree(int depth) override {
    for(int i=0;i<depth;++i) {
      std::cout<<" ";
    }
    std::cout<<depth<<":"<<"ElseStmt(declvars=";
    for(const std::string& var:declvars_) {
      std::cout<<var<<",";
    }
    std::cout<<")";
    std::cout<<std::endl;
    for(ASTNode* stmt:stmts_) {
      stmt->PrintTree(depth+1);
    }
  }
  virtual std::string GenJS() override {
    std::string result;
    result+="else{\n";
    for(std::string varname:declvars_) {
      result+="var "+varname+"=0.0;\n";
    }
    for(ASTNode* stmt:stmts_) {
      result+=stmt->GenJS();
    }
    result+="}\n";
    return result;
  }
};
} // namespace mocoder

#endif
