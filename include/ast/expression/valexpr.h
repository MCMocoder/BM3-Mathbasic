/**
 * @file valexpr.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-26
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef VALEXPR_H_
#define VALEXPR_H_

#include "ast/astnode.h"
#include "lex/lexer.h"
#include <cstdint>

namespace mocoder {
class Valexpr : public ASTNode {
public:
  int oper_;
  ASTNode *lchild_;
  ASTNode *rchild_;
  Valexpr(int oper, ASTNode *lchild, ASTNode *rchild)
      : oper_(oper), lchild_(lchild), rchild_(rchild) {}
  virtual ~Valexpr() {
    delete lchild_;
    delete rchild_;
  }

  virtual void PrintTree(int depth) override {
    for (int i = 0; i < depth; ++i) {
      std::cout << " ";
    }
    std::cout << depth << ":"
              << "Valexpr:";
    switch (oper_) {
    case Lexer::ADD:
      std::cout << "+";
      break;
    case Lexer::SUB:
      std::cout << "-";
      break;
    case Lexer::MUL:
      std::cout << "*";
      break;
    case Lexer::DIV:
      std::cout << "/";
      break;
    case Lexer::POW:
      std::cout << "^";
    }
    std::cout << std::endl;
    lchild_->PrintTree(depth + 1);
    rchild_->PrintTree(depth + 1);
  }

  virtual std::string GenJS() override {
    std::string result;
    result+="(";
    if(lchild_!=nullptr) {
      result+=lchild_->GenJS();
    }
    switch (oper_) {
    case Lexer::ADD:
      result+= "+";
      break;
    case Lexer::SUB:
      result+=  "-";
      break;
    case Lexer::MUL:
      result+= "*";
      break;
    case Lexer::DIV:
      result+=  "/";
      break;
    case Lexer::POW:
      result+= "**";
    }
    result+=rchild_->GenJS();
    result+=")";
    return result;
  }
};
} // namespace mocoder

#endif
