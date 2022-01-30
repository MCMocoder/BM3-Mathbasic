/**
 * @file rootnode.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ROOTNODE_H_
#define ROOTNODE_H_

#include "ast/astnode.h"
#include <string>
#include <unordered_set>

namespace mocoder {

class RootNode : public ASTNode {
public:
  std::list<ASTNode *> stmts_;
  std::unordered_set<std::string> declvars_;
  RootNode(std::list<ASTNode *> &stmts) : stmts_(std::move(stmts)) {}
  virtual ~RootNode() {
    for (ASTNode *stmt : stmts_) {
      delete stmt;
    }
  }

  virtual void PrintTree(int depth) override {
    for (int i = 0; i < depth; ++i) {
      std::cout << " ";
    }
    std::cout << depth << ":"
              << "RootNode";
    std::cout << std::endl;
    for (ASTNode *stmt : stmts_) {
      stmt->PrintTree(depth + 1);
    }
  }

  virtual std::string GenJS() override {
    std::string result = "function run() {\n";
    for(std::string varname:declvars_) {
      result+="var "+varname+"=0.0;\n";
    }
    for (ASTNode *stmt : stmts_) {
      result+=stmt->GenJS();
    }
    result += "}";
    return result;
  }
};

} // namespace mocoder

#endif
