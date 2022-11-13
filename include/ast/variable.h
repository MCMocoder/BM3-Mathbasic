/**
 * @file variable.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-11-02
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#pragma once

#include <cstddef>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace mocoder {

class Vars {
 public:
  std::list<std::unordered_map<std::string, double>> vars_;
  std::list<std::unordered_map<std::string, double>>::iterator scope_end_ =
      vars_.end();
  double retval_=0.0;
  static Vars& GetVars() {
    static Vars s;
    return s;
  }
  void EnterScope(const std::unordered_set<std::string>& declvars) {
    std::unordered_map<std::string, double> vs;
    for (auto& i : declvars) {
      vs.insert({i, 0.0});
    }
    vars_.push_front(std::move(vs));
  }
  void ExitScope() { vars_.pop_front(); }
  double GetVal(const std::string& name) {
    for (auto& i : vars_) {
      if (i.find(name) != i.end()) {
        return i.find(name)->second;
      }
    }
    return 0.0;
  }
  void SetVal(const std::string& name, const double val) {
    for (auto& i : vars_) {
      if (i.find(name) != i.end()) {
        i.find(name)->second = val;
        return;
      }
    }
    vars_.front().insert({name, val});
  }
};

}  // namespace mocoder
