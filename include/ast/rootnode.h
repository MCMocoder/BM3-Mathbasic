/**
 * @file rootnode.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-01-16
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#pragma once

#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "ast/astnode.h"
#include "ast/function.h"
#include "ast/statement/defstmt.h"
#include "ast/variable.h"

namespace mocoder {

class RootNode : public ASTNode {
 public:
  std::list<Ptr<ASTNode>> stmts_;
  std::list<Ptr<DefStmt>> defs_;
  std::unordered_set<std::string> declvars_;
  RootNode(std::list<Ptr<ASTNode>> &stmts) : stmts_(std::move(stmts)) {
    for (auto i=stmts_.begin();i!=stmts_.end();++i) {
      if ((*i)->IsDef()) {
        defs_.push_back(Ptr<DefStmt>((DefStmt *)(i->get())));
      }
    }
  }

  virtual void PrintTree(int depth) override {
    for (int i = 0; i < depth; ++i) {
      std::cout << " ";
    }
    std::cout << depth << ":"
              << "RootNode";
    std::cout << std::endl;
    for (Ptr<ASTNode> stmt : stmts_) {
      stmt->PrintTree(depth + 1);
    }
  }

  virtual std::string GenJS() override {
    std::string result = "function run() {";
    for (std::string varname : declvars_) {
      result += "var " + varname + "=0.0;";
    }
    for (Ptr<ASTNode> stmt : stmts_) {
      result += stmt->GenJS();
    }
    result += "}\n";
    return result;
  }

  virtual void Eval(Ptr<Vars> v) override {
    v=Ptr<Vars>(new Vars());
    v->EnterScope(declvars_);
    for (auto i : defs_) {
      Funcs::Get().AddFunc(i);
    }
    for (auto i : stmts_) {
      i->Eval(v);
    }
  }
};

}  // namespace mocoder
