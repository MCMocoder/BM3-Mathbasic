/**
 * @file valexpr.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-01-26
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#pragma once

#include <cstdint>

#include "ast/astnode.h"
#include "lex/lexer.h"

namespace mocoder {
class Valexpr : public ASTNode {
 public:
  int oper_;
  Ptr<Valexpr> lchild_;
  Ptr<Valexpr> rchild_;
  Valexpr() {}
  Valexpr(int oper, Ptr<Valexpr> lchild, Ptr<Valexpr> rchild)
      : oper_(oper), lchild_(lchild), rchild_(rchild) {}
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
    if (lchild_.get() != nullptr) {
      lchild_->PrintTree(depth + 1);
    }
    rchild_->PrintTree(depth + 1);
  }
  virtual std::string GenJS() override {
    std::string result;
    result += "(";
    if (lchild_ != nullptr) {
      result += lchild_->GenJS();
    }
    switch (oper_) {
      case Lexer::ADD:
        result += "+";
        break;
      case Lexer::SUB:
        result += "-";
        break;
      case Lexer::MUL:
        result += "*";
        break;
      case Lexer::DIV:
        result += "/";
        break;
      case Lexer::POW:
        result += "**";
    }
    result += rchild_->GenJS();
    result += ")";
    return result;
  }

  virtual double EvalVal(Ptr<Vars> v) {
    switch (oper_) {
      case Lexer::ADD:
        return lchild_->EvalVal(v) + rchild_->EvalVal(v);
      case Lexer::SUB:
        if (lchild_.get() == nullptr) {
          return -rchild_->EvalVal(v);
        }
        return lchild_->EvalVal(v) - rchild_->EvalVal(v);
      case Lexer::MUL:
        return lchild_->EvalVal(v) * rchild_->EvalVal(v);
      case Lexer::DIV:
        return lchild_->EvalVal(v) / rchild_->EvalVal(v);
      case Lexer::POW:
        return IntPow(lchild_->EvalVal(v), (int)(rchild_->EvalVal(v)));
    }
    return 0.0;
  }

  double IntPow(double x, int n) {
    if (n == 0) {
      return 1.0;
    } else if (n > 0) {
      double res = 1.0;
      double xprev = x;
      while (n != 0) {
        if (n % 2 != 0) {
          res *= xprev;
        }
        xprev = xprev * xprev;
        n /= 2;
      }
      return res;
    } else {
      double res = 1.0;
      double xprev = x;
      while (n != 0) {
        if (n % 2 != 0) {
          res *= xprev;
        }
        xprev = xprev * xprev;
        n /= 2;
      }
      return 1 / res;
    }
  }
  virtual void Eval(Ptr<Vars> v) override {}
  virtual void GenVM(Ptr<Vars> v, vector<Op>& ops) override {
    switch (oper_) {
      case Lexer::ADD:
        lchild_->GenVM(v,ops);
        rchild_->GenVM(v, ops);
        ops.push_back(Op(OpCode::ADD));
        break;
      case Lexer::SUB:
        if (lchild_.get() == nullptr) {
          ops.push_back(Op(OpCode::PUSH,0));
        } else {
          lchild_->GenVM(v,ops);
        }
        rchild_->GenVM(v, ops);
        ops.push_back(Op(OpCode::SUB));
        break;
      case Lexer::MUL:
        lchild_->GenVM(v, ops);
        rchild_->GenVM(v, ops);
        ops.push_back(Op(OpCode::MUL));
        break;
      case Lexer::DIV:
        lchild_->GenVM(v, ops);
        rchild_->GenVM(v, ops);
        ops.push_back(Op(OpCode::DIV));
        break;
      case Lexer::POW:
        lchild_->GenVM(v, ops);
        rchild_->GenVM(v, ops);
        ops.push_back(Op(OpCode::POW));
        break;
    }
  }
};

}  // namespace mocoder
