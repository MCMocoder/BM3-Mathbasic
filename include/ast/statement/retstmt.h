/**
 * @file retstmt.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-11-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "ast/astnode.h"
#include "ast/expression/callfuncexpr.h"
#include "ast/rootnode.h"

namespace mocoder {

class RetStmt : public ASTNode {
 public:
  Ptr<Valexpr> val_;
  RetStmt(Ptr<Valexpr> val) : val_(val) {}
  virtual void PrintTree(int depth) override {
    for (int i = 0; i < depth; ++i) {
      std::cout << " ";
    }
    std::cout << depth << ":"
              << "RetStmt" << std::endl;
    val_->PrintTree(depth + 1);
  }
  virtual std::string GenJS() override { return ""; }

  virtual void Eval(Ptr<Vars> v) override {
    v->retval_=val_->EvalVal(v);
  }
};

}  // namespace mocoder
