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

#ifndef INPUTSTMT_H_
#define INPUTSTMT_H_

#include "ast/astnode.h"
#include "ast/identifier/identifier.h"
//#include "ast/identifier/identifierlist.h"
#include "ast/variable.h"
#include "lex/lexer.h"


namespace mocoder {
class InputStmt : public ASTNode {
public:
  std::list<Ptr<Identifier>> vars_;
  InputStmt() {}

  void AddVar(Ptr<Identifier> id) {
    vars_.push_back(id);
  }

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
#ifdef DEBUG_MODE
      result += "=outside.input();\n";
#else
      result += "=outside.input();";
#endif
    }
    return result;
  }

  virtual void Eval() override {
    for (auto i : vars_) {
      double d;
      scanf("%lf", &d);
      Vars::GetVars().SetVal(i->name_, d);
    }
  }
};
} // namespace mocoder

#endif
