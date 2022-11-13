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

#pragma once

#include <list>
#include <string>
#include <unordered_set>

#include "ast/astnode.h"
#include "ast/expression/condexpr.h"
#include "ast/statement/elsestmt.h"
#include "lex/lexer.h"

namespace mocoder {
class IfStmt : public ASTNode {
 public:
  Ptr<Condexpr> cond_;
  std::list<Ptr<ASTNode>> stmts_;
  Ptr<ElseStmt> else_;
  std::unordered_set<std::string> declvars_;
  IfStmt(Ptr<Condexpr> cond, std::list<Ptr<ASTNode>> &stmts)
      : cond_(cond), stmts_(stmts) {
    else_ = nullptr;
  }
  virtual void PrintTree(int depth) override {
    for (int i = 0; i < depth; ++i) {
      std::cout << " ";
    }
    std::cout << depth << ":"
              << "IfStmt(declvars=";
    for (const std::string &var : declvars_) {
      std::cout << var << ",";
    }
    std::cout << ")";
    std::cout << std::endl;
    cond_->PrintTree(depth + 1);
    for (Ptr<ASTNode> stmt : stmts_) {
      stmt->PrintTree(depth + 1);
    }
    if (else_ != nullptr) {
      else_->PrintTree(depth + 1);
    }
  }
  virtual std::string GenJS() override {
    std::string result;
    result += "if(";
    result += cond_->GenJS();
    result += "){";
    for (std::string varname : declvars_) {
      result += "var " + varname + "=0.0;";
    }
    for (Ptr<ASTNode> stmt : stmts_) {
      result += stmt->GenJS();
    }
    if (else_ != nullptr) {
      result += "}";
      result += else_->GenJS();
    } else {
      result += "}";
    }
    return result;
  }

  virtual void Eval(Ptr<Vars> v) override {
    if (cond_->EvalCond(v)) {
      v->EnterScope(declvars_);
      for (auto i : stmts_) {
        i->Eval(v);
      }
      v->ExitScope();
    } else if (else_ != nullptr) {
      else_->Eval(v);
    }
  }
};
}  // namespace mocoder
