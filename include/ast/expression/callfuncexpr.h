/**
 * @file callfuncexpr.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-01-22
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#pragma once

#include <cmath>

#include "ast/astnode.h"
#include "ast/expression/valexpr.h"
#include "ast/expression/valexprlist.h"
#include "ast/function.h"
#include "ast/identifier/identifier.h"

namespace mocoder {
class CallFuncExpr : public Valexpr {
 public:
  std::string name_;
  Ptr<ValexprList> params_;
  CallFuncExpr(std::string &name, Ptr<ValexprList> params)
      : name_(name), params_(params) {}
  virtual void PrintTree(int depth) override {
    for (int i = 0; i < depth; ++i) {
      std::cout << " ";
    }
    std::cout << depth << ":"
              << "CallFuncExpr(name=" << name_ << ")";
    std::cout << std::endl;
    params_->PrintTree(depth + 1);
  }
  virtual std::string GenJS() override {
    std::string result;
    result += "Math.";  // All Internal functions are from Javascript.Math
    result += name_;
    result += "(";
    result += params_->GenJS();
    result += ")";
    return result;
  }
  virtual double EvalVal(Ptr<Vars> v) override {
    if (name_ == "cos") {
      return cos((*(params_->exprs_.begin()))->EvalVal(v));
    } else if (name_ == "sin") {
      return sin((*(params_->exprs_.begin()))->EvalVal(v));
    } else if (name_ == "tan") {
      return tan((*(params_->exprs_.begin()))->EvalVal(v));
    } else if (name_ == "ln") {
      return log((*(params_->exprs_.begin()))->EvalVal(v));
    } else if (name_ == "lg") {
      return log10((*(params_->exprs_.begin()))->EvalVal(v));
    } else if (name_ == "log") {
      return log((*(params_->exprs_.begin()))->EvalVal(v)) /
             log((*(std::next(params_->exprs_.begin(), 1)))->EvalVal(v));
    } else {
      return Funcs::Get().GetFunc(name_)->EvalFunc(params_,v);
    }
  }
  virtual void Eval(Ptr<Vars> v) override {}
};
}  // namespace mocoder
