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

#pragma once

#include <list>

#include "ast/astnode.h"
#include "ast/expression/condexpr.h"
#include "ast/variable.h"
#include "lex/lexer.h"

namespace mocoder {
class DoLoopStmt : public ASTNode {
 public:
  Ptr<Condexpr> cond_;
  std::list<Ptr<ASTNode>> stmts_;
  std::unordered_set<std::string> declvars_;
  DoLoopStmt(Ptr<Condexpr> cond, std::list<Ptr<ASTNode>> &stmts)
      : cond_(cond), stmts_(std::move(stmts)) {}
  virtual void PrintTree(int depth) override {
    for (int i = 0; i < depth; ++i) {
      std::cout << " ";
    }
    std::cout << depth << ":"
              << "DoLoopStmt(declvars=";
    for (const std::string &var : declvars_) {
      std::cout << var << ",";
    }
    std::cout << ")";
    std::cout << std::endl;
    cond_->PrintTree(depth + 1);
    for (Ptr<ASTNode> stmt : stmts_) {
      stmt->PrintTree(depth + 1);
    }
  }
  virtual std::string GenJS() override {
    std::string result;
    result += "do{";
    for (std::string varname : declvars_) {
      result += "var " + varname + "=0.0;";
    }
    for (Ptr<ASTNode> stmt : stmts_) {
      result += stmt->GenJS();
    }
    result += "}while(";
    result += cond_->GenJS();
    result += ");";
    return result;
  }
  virtual void Eval(Ptr<Vars> v) override {
    v->EnterScope(declvars_);
    do {
      for (auto i : stmts_) {
        i->Eval(v);
      }
    } while (!cond_->EvalCond(v));
    v->ExitScope();
  }
  virtual void GenVM(Ptr<Vars> v, vector<Op> &ops) override {
    int jmploc = ops.size();
    for (auto i : stmts_) {
      i->GenVM(v, ops);
    }
    cond_->GenVM(v, ops);
    ops.push_back(Op(OpCode::JNZ, jmploc));
  }
};
}  // namespace mocoder
