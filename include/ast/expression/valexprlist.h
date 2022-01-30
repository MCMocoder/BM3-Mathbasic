/**
 * @file valexprlist.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-22
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef VALEXPRLIST_H_
#define VALEXPRLIST_H_

#include "ast/astnode.h"
#include "ast/expression/valexpr.h"
#include <algorithm>
#include <list>

namespace mocoder {
class ValexprList : public ASTNode {
public:
  std::list<ASTNode *> exprs_;
  ValexprList() {}
  void AddExpr(ASTNode *expr) { exprs_.push_back(expr); }
  virtual ~ValexprList() {
    for (ASTNode *expr : exprs_) {
      delete expr;
    }
  }
  virtual void PrintTree(int depth) override {
    for (int i = 0; i < depth; ++i) {
      std::cout << " ";
    }
    std::cout << depth << ":"
              << "Valexprlist";
    std::cout<<std::endl;          
    for (ASTNode *stmt : exprs_) {
      stmt->PrintTree(depth + 1);
    }
  }
  virtual std::string GenJS() override {
    std::string result;
    for(ASTNode* expr:exprs_) {
      result+=expr->GenJS();
      result+=",";
    }
    result.erase(result.size()-1);
    return result;
  }
};
} // namespace mocoder

#endif
