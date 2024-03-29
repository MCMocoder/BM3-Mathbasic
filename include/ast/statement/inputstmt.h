/**
 * @file inputstmt.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-01-22
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#pragma once

#include "ast/astnode.h"
#include "ast/identifier/identifier.h"
#include "ast/variable.h"
#include "lex/lexer.h"

namespace mocoder {
class InputStmt : public ASTNode {
 public:
  std::list<Ptr<Identifier>> vars_;
  InputStmt() {}

  void AddVar(Ptr<Identifier> id) { vars_.push_back(id); }

  virtual void PrintTree(int depth) override {
    for (int i = 0; i < depth; ++i) {
      std::cout << " ";
    }
    std::cout << depth << ":"
              << "InputStmt";
    std::cout << std::endl;
    for (auto i : vars_) {
      i->PrintTree(depth + 1);
    }
  }
  virtual std::string GenJS() override {
    std::string result;
    for (Ptr<ASTNode> node : vars_) {
      result += node->GenJS();
      result += "=outside.input();";
    }
    return result;
  }
  virtual void Eval(Ptr<Vars> v) override {
    for (auto i : vars_) {
      double d;
      scanf("%lf", &d);
      v->SetVal(i->name_, d);
    }
  }
  virtual void GenVM(Ptr<Vars> v, vector<Op> &ops) override {
    for (auto i : vars_) {
      ops.push_back(Op(OpCode::INPUT, i->name_));
    }
  }
};
}  // namespace mocoder
