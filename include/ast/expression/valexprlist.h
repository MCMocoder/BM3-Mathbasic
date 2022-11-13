/**
 * @file valexprlist.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-01-22
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#pragma once

#include <algorithm>
#include <list>

#include "ast/astnode.h"
#include "ast/expression/valexpr.h"

namespace mocoder {
class ValexprList : public ASTNode {
 public:
  std::list<Ptr<Valexpr>> exprs_;
  ValexprList() {}
  void AddExpr(Ptr<Valexpr> expr) { exprs_.push_back(expr); }
  virtual void PrintTree(int depth) override {
    for (int i = 0; i < depth; ++i) {
      std::cout << " ";
    }
    std::cout << depth << ":"
              << "Valexprlist";
    std::cout << std::endl;
    for (Ptr<Valexpr> stmt : exprs_) {
      if (stmt != nullptr) {
        stmt->PrintTree(depth + 1);
      }
    }
  }
  virtual std::string GenJS() override {
    std::string result;
    for (Ptr<Valexpr> expr : exprs_) {
      result += expr->GenJS();
      result += ",";
    }
    result.erase(result.size() - 1);
    return result;
  }
  virtual std::list<double> EvalVList(Ptr<Vars> v) {
    std::list<double> res;
    for (auto i : exprs_) {
      res.push_back(i->EvalVal(v));
    }
    return res;
  }
  virtual void Eval(Ptr<Vars> v) override {}
};
}  // namespace mocoder
