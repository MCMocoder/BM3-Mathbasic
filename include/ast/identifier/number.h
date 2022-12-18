/**
 * @file number.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-01-22
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#pragma once

#include <string>

#include "ast/astnode.h"
#include "ast/expression/valexpr.h"
#include "lex/lexer.h"

namespace mocoder {
class Number : public Valexpr {
 public:
  double val_;
  Number(const double& val) : val_(val) {}
  virtual void PrintTree(int depth) override {
    for (int i = 0; i < depth; ++i) {
      std::cout << " ";
    }
    std::cout << depth << ":"
              << "Number(val=" << val_ << ")";
    std::cout << std::endl;
  }
  virtual std::string GenJS() override { return std::to_string(val_); }
  virtual double EvalVal(Ptr<Vars> v) override { return val_; }
  virtual void Eval(Ptr<Vars> v) override {}
  virtual void GenVM(Ptr<Vars> v, vector<Op>& ops) override {
    ops.push_back(Op(OpCode::PUSH, val_));
  }
};
}  // namespace mocoder
