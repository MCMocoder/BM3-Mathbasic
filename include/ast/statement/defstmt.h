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

#include <iterator>
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
  virtual bool IsArg(string name) {
    for (auto j : args_) {
      if (j->name_ == name) {
        return true;
      }
    }
    return false;
  }
  virtual std::string GenJS() override {
    std::string result;
    result += "function ";
    result += name_;
    result += "(";
    for (auto i : args_) {
      result += (i->name_ + ",");
    }
    result.erase(result.end() - 1);
    result += ")";
    result += "{";
    for (auto i : declvars_) {
      if (!IsArg(i)) {
        result += "var " + i + "=0.0;";
      }
    }
    for (auto i : stmts_) {
      result += i->GenJS();
    }
    result += "}";
    return result;
  }

  virtual void Eval(Ptr<Vars> v) override {}

  virtual bool IsDef() override { return true; }

  double EvalFunc(Ptr<ValexprList> argin, Ptr<Vars> pv) {
    Ptr<Vars> v(new Vars());
    std::list<double> argvs = argin->EvalVList(pv);
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

  virtual void GenVM(Ptr<Vars> v, vector<Op> &ops) override {
    for (auto i = args_.rbegin(); i != args_.rend(); ++i) {
      ops.push_back(Op(OpCode::STORE, (*i)->name_));
    }
    for (auto i : stmts_) {
      i->GenVM(v, ops);
    }
  }
};

}  // namespace mocoder
