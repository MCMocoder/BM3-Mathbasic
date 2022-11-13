/**
 * @file function.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-11-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <cstddef>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "ast/statement/defstmt.h"

namespace mocoder {

class Funcs {
 public:
  std::unordered_map<std::string, Ptr<DefStmt>> funcs_;
  static Funcs& Get() {
    static Funcs f;
    return f;
  }
  Ptr<DefStmt> GetFunc(const std::string& name) {
    return funcs_.find(name)->second;
  }
  void AddFunc(Ptr<DefStmt> f) {
    funcs_.insert({f->name_,f});
  }
};

}
