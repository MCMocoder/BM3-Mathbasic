/**
 * @file vm.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-12-14
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#pragma once

#include <iostream>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

namespace mocoder {

using namespace std;

enum class OpCode {
  ADD,
  SUB,
  MUL,
  DIV,
  POW,
  PUSH,
  POP,
  CALL,
  EQ,
  NEQ,
  LESS,
  MORE,
  LESSEQ,
  MOREEQ,
  JMP,
  JZ,
  JNZ,
  LOAD,
  STORE,
  CALLSIN,
  CALLCOS,
  CALLTAN,
  CALLLN,
  CALLLG,
  CALLLOG,
  INPUT,
  PRINT,
  RET
};

struct Op {
  OpCode code_;
  string param_;
  int val_;
  Op(OpCode code) : code_(code), param_("") {}
  Op(OpCode code, string param) : code_(code), param_(param) {}
  Op(OpCode code, int val) : code_(code), val_(val) {}
};

class VarTable {
 public:
  map<string, double> vars_;
  double GetVal(const std::string& name) {
    if (vars_.find(name) != vars_.end()) {
      return vars_.find(name)->second;
    }
    return 0.0;
  }
  void SetVal(const std::string& name, const double val) {
    if (vars_.find(name) != vars_.end()) {
      vars_.find(name)->second = val;
      return;
    }
    vars_.emplace(name, val);
  };
};

class VM {
 public:
  template <typename T>
  using Ptr = std::shared_ptr<T>;
  vector<Op> main_;
  map<string, vector<Op>> funcs_;
  stack<double> stack_;
  bool reg_zf_;
  int calldep_ = 0;

  void RunFunc();

  void Add() {
    double b = stack_.top();
    stack_.pop();
    double a = stack_.top();
    stack_.pop();
    stack_.push(a + b);
  }
  void Sub() {
    double b = stack_.top();
    stack_.pop();
    double a = stack_.top();
    stack_.pop();
    stack_.push(a - b);
  }
  void Mul() {
    double b = stack_.top();
    stack_.pop();
    double a = stack_.top();
    stack_.pop();
    stack_.push(a - b);
  }
  void Div() {
    double b = stack_.top();
    if (b == 0) {
      std::__throw_logic_error("Cannot divide a number by zero");
    }
    stack_.pop();
    double a = stack_.top();
    stack_.pop();
    stack_.push(a / b);
  }
  void Pow() {
    double b = stack_.top();
    stack_.pop();
    double a = stack_.top();
    stack_.pop();
    stack_.push(IntPow(a, b));
  }
  void Push(double x) { stack_.push(x); }
  void Pop() { stack_.pop(); }
  void Call(string name) { RunFunc(name); }
  void Eq() {
    double b = stack_.top();
    stack_.pop();
    double a = stack_.top();
    stack_.pop();
    reg_zf_ = (a == b);
  }
  void Neq() {
    double b = stack_.top();
    stack_.pop();
    double a = stack_.top();
    stack_.pop();
    reg_zf_ = (a != b);
  }
  void Less() {
    double b = stack_.top();
    stack_.pop();
    double a = stack_.top();
    stack_.pop();
    reg_zf_ = (a < b);
  }
  void More() {
    double b = stack_.top();
    stack_.pop();
    double a = stack_.top();
    stack_.pop();
    reg_zf_ = (a > b);
  }
  void Lesseq() {
    double b = stack_.top();
    stack_.pop();
    double a = stack_.top();
    stack_.pop();
    reg_zf_ = (a <= b);
  }
  void Moreeq() {
    double b = stack_.top();
    stack_.pop();
    double a = stack_.top();
    stack_.pop();
    reg_zf_ = (a >= b);
  }
  void Jmp(int val, int& next) { next = val - 1; }
  void Jz(int val, int& next) {
    if (!reg_zf_) {
      next = val - 1;
    }
  }
  void Jnz(int val, int& next) {
    if (reg_zf_) {
      next = val - 1;
    }
  }
  void Load(string name, VarTable& v) { stack_.push(v.GetVal(name)); }
  void Store(string name, VarTable& v) {
    v.SetVal(name, stack_.top());
    stack_.pop();
  }
  void Callsin() {
    double a = stack_.top();
    stack_.pop();
    stack_.push(sin(a));
  }
  void Callcos() {
    double a = stack_.top();
    stack_.pop();
    stack_.push(cos(a));
  }
  void Calltan() {
    double a = stack_.top();
    stack_.pop();
    stack_.push(tan(a));
  }
  void Callln() {
    double a = stack_.top();
    stack_.pop();
    stack_.push(log(a));
  }
  void Calllg() {
    double a = stack_.top();
    stack_.pop();
    stack_.push(log10(a));
  }
  void Calllog() {
    double b = stack_.top();
    stack_.pop();
    double a = stack_.top();
    stack_.pop();
    stack_.push(log(a) / log(b));
  }
  void Input(string name, VarTable& v) {
    double t;
    cin >> t;
    v.SetVal(name, t);
  }
  void Print(string name, VarTable& v) {
    cout << stack_.top() << endl;
    stack_.pop();
  }
  void Ret() { calldep_ -= 1; }
  void RunOp(Op op, int& next, VarTable& v) {
    switch (op.code_) {
      case OpCode::ADD:
        Add();
        break;
      case OpCode::SUB:
        Sub();
        break;
      case OpCode::MUL:
        Mul();
        break;
      case OpCode::DIV:
        Div();
        break;
      case OpCode::POW:
        Pow();
        break;
      case OpCode::PUSH:
        Push(op.val_);
        break;
      case OpCode::POP:
        Pop();
        break;
      case OpCode::CALL:
        Call(op.param_);
        break;
      case OpCode::EQ:
        Eq();
        break;
      case OpCode::NEQ:
        Neq();
        break;
      case OpCode::LESS:
        Less();
        break;
      case OpCode::MORE:
        More();
        break;
      case OpCode::LESSEQ:
        Lesseq();
        break;
      case OpCode::MOREEQ:
        Moreeq();
        break;
      case OpCode::JMP:
        Jmp(op.val_, next);
        break;
      case OpCode::JZ:
        Jz(op.val_, next);
        break;
      case OpCode::JNZ:
        Jnz(op.val_, next);
        break;
      case OpCode::LOAD:
        Load(op.param_, v);
        break;
      case OpCode::STORE:
        Store(op.param_, v);
        break;
      case OpCode::CALLSIN:
        Callsin();
        break;
      case OpCode::CALLCOS:
        Callcos();
        break;
      case OpCode::CALLTAN:
        Calltan();
        break;
      case OpCode::CALLLN:
        Callln();
        break;
      case OpCode::CALLLG:
        Calllg();
        break;
      case OpCode::CALLLOG:
        Calllog();
        break;
      case OpCode::INPUT:
        Input(op.param_, v);
        break;
      case OpCode::PRINT:
        Print(op.param_, v);
        break;
      case OpCode::RET:
        Ret();
        break;
    }
  }
  void RunFunc(string funcname) {
    ++calldep_;
    int dep = calldep_;
    VarTable v;
    vector<Op>& func = funcs_.find(funcname)->second;
    int next = 0;
    while (next < func.size()) {
      if (dep != calldep_) {
        return;
      }
      RunOp(func[next], next, v);
      ++next;
    }
    --calldep_;
  }
  void Run() {
    VarTable v;
    int next = 0;
    while (next < main_.size()) {
      RunOp(main_[next], next, v);
      ++next;
    }
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
};

}  // namespace mocoder
