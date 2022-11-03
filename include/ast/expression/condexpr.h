/**
 * @file condexpr.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-01-26
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
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
  Ptr<Valexpr> lexpr_;
  Ptr<Valexpr> rexpr_;
  Condexpr(int oper, Ptr<Valexpr> lexpr, Ptr<Valexpr> rexpr)
      : oper_(oper), lexpr_(lexpr), rexpr_(rexpr) {}
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
  virtual bool EvalCond() {
    switch (oper_) {
    case Lexer::GREATEREQUAL:
      return lexpr_->EvalVal()>=rexpr_->EvalVal();
    case Lexer::GREATER:
      return lexpr_->EvalVal()>rexpr_->EvalVal();
    case Lexer::LESSEQUAL:
      return lexpr_->EvalVal()<=rexpr_->EvalVal();
    case Lexer::LESS:
      return lexpr_->EvalVal()<rexpr_->EvalVal();
    case Lexer::NOEQUAL:
      return lexpr_->EvalVal()!=rexpr_->EvalVal();
    case Lexer::EQUAL:
      return lexpr_->EvalVal()==rexpr_->EvalVal();
    }
    return false;
  }
  virtual void Eval() override {}
};
} // namespace mocoder

#endif
