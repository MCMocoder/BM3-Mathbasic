/**
 * @file defstmt.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-11-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <list>
#include <memory>
#include <stdexcept>

#include "ast/astnode.h"
#include "ast/expression/valexprlist.h"
#include "ast/identifier/identifier.h"
#include "ast/variable.h"

namespace mocoder {

class DefStmt : public ASTNode, std::enable_shared_from_this<DefStmt> {
 public:
  std::list<Ptr<ASTNode>> stmts_;
  std::string name_;
  std::list<Ptr<Identifier>> args_;
  std::unordered_set<std::string> declvars_;
  DefStmt(std::list<Ptr<ASTNode>> stmts, std::string name,
          std::list<Ptr<Identifier>> args)
      : stmts_(stmts), name_(name), args_(args) {}
  virtual void PrintTree(int depth) override {
    for (int i = 0; i < depth; ++i) {
      std::cout << " ";
    }
    std::cout << depth << ":"
              << "DefStmt(name=" << name_ << ")" << std::endl;
    for (auto &i : args_) {
      i->PrintTree(depth + 1);
    }
    for (auto &i : stmts_) {
      i->PrintTree(depth + 1);
    }
  }
  virtual std::string GenJS() override { return ""; }

  virtual void Eval(Ptr<Vars> v) override {}

  virtual bool IsDef() override { return true; }

  double EvalFunc(Ptr<ValexprList> argin, Ptr<Vars> pv) {
    Ptr<Vars> v(new Vars());
    std::list<double> argvs = argin->EvalVList(pv);
    if (args_.size() != argvs.size()) {
      std::__throw_logic_error("Unexpected Param Number");
    }
    v->EnterScope(declvars_);
    auto j = argvs.begin();
    for (auto i = args_.begin(); i != args_.end(); ++i) {
      v->SetVal(i->get()->name_, *j);
      ++j;
    }
    for (auto i : stmts_) {
      i->Eval(v);
    }
    v->ExitScope();
    return v->retval_;
  }
};

}  // namespace mocoder
