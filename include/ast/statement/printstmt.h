/**
 * @file printstmt.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-01-22
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#pragma once

#include <iostream>
#include <string>

#include "ast/astnode.h"
#include "ast/expression/valexprlist.h"
#include "lex/lexer.h"

namespace mocoder {
class PrintStmt : public ASTNode {
 public:
  Ptr<ValexprList> exprs_;
  PrintStmt(Ptr<ValexprList> exprs) : exprs_(exprs) {}

  virtual void PrintTree(int depth) override {
    for (int i = 0; i < depth; ++i) {
      std::cout << " ";
    }
    std::cout << depth << ":"
              << "PrintStmt";
    std::cout << std::endl;
    exprs_->PrintTree(depth + 1);
  }
  virtual std::string GenJS() override {
    std::string result;
    for (Ptr<ASTNode> node : exprs_->exprs_) {
      result += "outside.print(";
      result += node->GenJS();
      result += ");";
    }
    return result;
  }
  virtual void Eval(Ptr<Vars> v) override {
    std::list<double> vals = exprs_->EvalVList(v);
    for (auto i : vals) {
      printf("%lf\n", i);
    }
  }
  virtual void GenVM(Ptr<Vars> v, vector<Op> &ops) override {
    for (auto i : exprs_->exprs_) {
      i->GenVM(v, ops);
      ops.push_back(Op(OpCode::PRINT));
    }
  }
};
}  // namespace mocoder
