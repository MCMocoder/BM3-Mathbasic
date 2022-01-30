/**
 * @file callfuncexpr.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-22
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef CALLFUNCEXPR_H_
#define CALLFUNCEXPR_H_

#include "ast/astnode.h"
#include "ast/expression/valexprlist.h"
#include "ast/identifier/identifier.h"

namespace mocoder {
class CallFuncExpr : public ASTNode {
public:
  std::string name_;
  ValexprList *params_;
  CallFuncExpr(std::string &name, ValexprList *params)
      : name_(name), params_(params) {}
  virtual ~CallFuncExpr() { delete params_; }
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
    result += "Math.";//All Internal functions are from Javascript.Math
    result += name_;
    result += "(";
    result += params_->GenJS();
    result += ")";
    return result;
  }
};
} // namespace mocoder

#endif
