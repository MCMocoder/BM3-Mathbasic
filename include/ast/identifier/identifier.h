/**
 * @file identifier.h
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
#include "ast/variable.h"
#include "lex/lexer.h"

namespace mocoder {
class Identifier : public Valexpr {
 public:
  std::string name_;
  Identifier(const std::string& name) : name_(name) {}
  virtual void PrintTree(int depth) override {
    for (int i = 0; i < depth; ++i) {
      std::cout << " ";
    }
    std::cout << depth << ":"
              << "Identifier(name=" << name_ << ")";
    std::cout << std::endl;
  }
  virtual std::string GenJS() override { return name_; }
  virtual double EvalVal(Ptr<Vars> v) override { return v->GetVal(name_); }
  virtual void Eval(Ptr<Vars> v) override {}
  virtual void GenVM(Ptr<Vars> v, vector<Op>& ops) override {
    ops.push_back(Op(OpCode::LOAD, name_));
  }
};
}  // namespace mocoder
