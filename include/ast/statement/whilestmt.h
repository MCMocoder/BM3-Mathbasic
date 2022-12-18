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

#pragma once

#include <list>

#include "ast/astnode.h"
#include "ast/expression/condexpr.h"
#include "lex/lexer.h"

namespace mocoder {
class WhileStmt : public ASTNode {
 public:
  Ptr<Condexpr> cond_;
  std::list<Ptr<ASTNode>> stmts_;
  std::unordered_set<std::string> declvars_;
  WhileStmt(Ptr<Condexpr> cond, std::list<Ptr<ASTNode>> &stmts)
      : cond_(cond), stmts_(stmts) {}
  virtual void PrintTree(int depth) override {
    for (int i = 0; i < depth; ++i) {
      std::cout << " ";
    }
    std::cout << depth << ":"
              << "WhileStmt(declvars=";
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
    result += "if(";
    result += cond_->GenJS();
    result += "){";
    for (std::string varname : declvars_) {
      result += "var " + varname + "=0.0;";
    }
    for (Ptr<ASTNode> stmt : stmts_) {
      result += stmt->GenJS();
    }
    result += "}";
    return result;
  }
  virtual void Eval(Ptr<Vars> v) override {
    while (cond_->EvalCond(v)) {
      for (auto i : stmts_) {
        i->Eval(v);
      }
    }
  }
  virtual void GenVM(Ptr<Vars> v, vector<Op> &ops) override {
    int looploc = ops.size();
    cond_->GenVM(v, ops);
    vector<Op> content;
    for (auto i : stmts_) {
      i->GenVM(v, content);
    }
    content.push_back(Op(OpCode::JMP, looploc));
    ops.push_back(Op(OpCode::JZ, ops.size() + content.size() + 1));
    ops.insert(ops.end(), content.begin(), content.end());
  }
};
}  // namespace mocoder
