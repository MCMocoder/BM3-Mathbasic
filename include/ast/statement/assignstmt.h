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
#include "lex/lexer.h"

namespace mocoder {
class AssignStmt : public ASTNode {
public:
  Identifier *var_;
  ASTNode *expr_;
  AssignStmt(Identifier *var, ASTNode *expr) : var_(var), expr_(expr) {}
  virtual ~AssignStmt() {
    delete var_;
    delete expr_;
  }
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
};
} // namespace mocoder

#endif
