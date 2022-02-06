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
#include <string>
#include <unordered_set>

namespace mocoder {

class RootNode : public ASTNode {
public:
  std::list<Ptr<ASTNode> > stmts_;
  std::unordered_set<std::string> declvars_;
  RootNode(std::list<Ptr<ASTNode> > &stmts) : stmts_(std::move(stmts)) {}

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
    #ifdef DEBUG_MODE
    std::string result = "function run() {\n";
    #else 
    std::string result = "function run() {";
    #endif
    for(std::string varname:declvars_) {
      #ifdef DEBUG_MODE
      result+="var "+varname+"=0.0;\n";
      #else
      result+="var "+varname+"=0.0;";
      #endif
    }
    for (Ptr<ASTNode> stmt : stmts_) {
      result+=stmt->GenJS();
    }
    result += "}\n";
    return result;
  }
};

} // namespace mocoder

#endif
