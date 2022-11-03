/**
 * @file assignstmt.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-01-22
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#ifndef ASSIGNSTMT_H_
#define ASSIGNSTMT_H_

#include "ast/astnode.h"
#include "ast/expression/valexpr.h"
#include "ast/identifier/identifier.h"
#include "ast/variable.h"
#include "lex/lexer.h"

namespace mocoder {
class AssignStmt : public ASTNode {
 public:
  Ptr<Identifier> var_;
  Ptr<Valexpr> expr_;
  AssignStmt(Ptr<Identifier> var, Ptr<Valexpr> expr) : var_(var), expr_(expr) {}
  virtual void PrintTree(int depth) override {
    for (int i = 0; i < depth; ++i) {
      std::cout << " ";
    }
    std::cout << depth << ":"
              << "AssignStmt";
    std::cout << std::endl;
    var_->PrintTree(depth + 1);
    expr_->PrintTree(depth + 1);
  }
  virtual std::string GenJS() override {
    std::string result;
    result += var_->GenJS();
    result += "=";
    result += expr_->GenJS();
#ifdef DEBUG_MODE
    result += ";\n";
#else
    result += ";";
#endif
    return result;
  }
  virtual double Value() { return 0; }
  virtual void Eval() override {
    Vars::GetVars().SetVal(var_->name_, expr_->EvalVal());
  }
  /*virtual void Eval(Ptr<ASTNode> parent) override {
    expr_->Eval(shared_from_this());
    parent->GetVars().insert({var_->name_,expr_->GetVal()});
  }*/
};
}  // namespace mocoder

#endif
