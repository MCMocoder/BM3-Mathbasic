/**
 * @file condexpr.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-26
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef CONDEXPR_H_
#define CONDEXPR_H_

#include "ast/astnode.h"
#include "ast/expression/valexpr.h"
#include "lex/lexer.h"

namespace mocoder {
class Condexpr : public ASTNode {
public:
  int oper_;
  ASTNode *lexpr_;
  ASTNode *rexpr_;
  Condexpr(int oper, ASTNode *lexpr, ASTNode *rexpr)
      : oper_(oper), lexpr_(lexpr), rexpr_(rexpr) {}
  virtual ~Condexpr() {
    delete lexpr_;
    delete rexpr_;
  }
  virtual void PrintTree(int depth) override {
    for (int i = 0; i < depth; ++i) {
      std::cout << " ";
    }
    std::cout << depth << ":"
              << "Condexpr(oper=";
    switch (oper_) {
    case Lexer::GREATEREQUAL:
      std::cout << ">=";
      break;
    case Lexer::GREATER:
      std::cout << ">";
      break;
    case Lexer::LESSEQUAL:
      std::cout << "<=";
      break;
    case Lexer::LESS:
      std::cout << "<";
      break;
    case Lexer::NOEQUAL:
      std::cout << "!=";
      break;
    case Lexer::EQUAL:
      std::cout << "===";
    }
    std::cout << ")";
    std::cout << std::endl;
    lexpr_->PrintTree(depth + 1);
    rexpr_->PrintTree(depth + 1);
  }
  virtual std::string GenJS() override {
    std::string result;
    result += lexpr_->GenJS();
    switch (oper_) {
    case Lexer::GREATEREQUAL:
      result += ">=";
      break;
    case Lexer::GREATER:
      result += ">";
      break;
    case Lexer::LESSEQUAL:
      result += "<=";
      break;
    case Lexer::LESS:
      result += "<";
      break;
    case Lexer::NOEQUAL:
      result += "!=";
      break;
    case Lexer::EQUAL:
      result += "===";
    }
    result += rexpr_->GenJS();
    return result;
  }
};
} // namespace mocoder

#endif
