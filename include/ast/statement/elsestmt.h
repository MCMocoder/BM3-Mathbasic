/**
 * @file elsestmt.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-01-22
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#ifndef ELSESTMT_H_
#define ELSESTMT_H_

#include "ast/astnode.h"
#include "ast/expression/condexpr.h"
#include "ast/variable.h"
#include "lex/lexer.h"
#include <list>
#include <string>
#include <unordered_set>


namespace mocoder {
class ElseStmt : public ASTNode {
public:
  std::list<Ptr<ASTNode>> stmts_;
  std::unordered_set<std::string> declvars_;
  ElseStmt(std::list<Ptr<ASTNode>> &stmts) : stmts_(std::move(stmts)) {}
  virtual void PrintTree(int depth) override {
    for (int i = 0; i < depth; ++i) {
      std::cout << " ";
    }
    std::cout << depth << ":"
              << "ElseStmt(declvars=";
    for (const std::string &var : declvars_) {
      std::cout << var << ",";
    }
    std::cout << ")";
    std::cout << std::endl;
    for (Ptr<ASTNode> stmt : stmts_) {
      stmt->PrintTree(depth + 1);
    }
  }
  virtual std::string GenJS() override {
    std::string result;
    result += "else{";
    for (std::string varname : declvars_) {
      result += "var " + varname + "=0.0;";
    }
    for (Ptr<ASTNode> stmt : stmts_) {
      result += stmt->GenJS();
    }
    result += "}";
    return result;
  }

  virtual void Eval() override {
    Vars::GetVars().EnterScope(declvars_);
    for (auto i : stmts_) {
      i->Eval();
    }
    Vars::GetVars().ExitScope();
  }
};
} // namespace mocoder

#endif
