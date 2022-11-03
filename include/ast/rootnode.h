/**
 * @file rootnode.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-01-16
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#ifndef ROOTNODE_H_
#define ROOTNODE_H_

#include "ast/astnode.h"
#include "ast/variable.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>


namespace mocoder {

class RootNode : public ASTNode {
public:
  std::list<Ptr<ASTNode>> stmts_;
  std::unordered_set<std::string> declvars_;
  std::unordered_map<std::string, double> vals_;
  RootNode(std::list<Ptr<ASTNode>> &stmts) : stmts_(std::move(stmts)) {}

  virtual void PrintTree(int depth) override {
    for (int i = 0; i < depth; ++i) {
      std::cout << " ";
    }
    std::cout << depth << ":"
              << "RootNode";
    std::cout << std::endl;
    for (Ptr<ASTNode> stmt : stmts_) {
      stmt->PrintTree(depth + 1);
    }
  }

  virtual std::string GenJS() override {
    std::string result = "function run() {";
    for (std::string varname : declvars_) {
      result += "var " + varname + "=0.0;";
    }
    for (Ptr<ASTNode> stmt : stmts_) {
      result += stmt->GenJS();
    }
    result += "}\n";
    return result;
  }

  virtual void Eval() override {
    Vars::GetVars().EnterScope(declvars_);
    for (auto i : stmts_) {
      i->Eval();
    }
  }
};

} // namespace mocoder

#endif
