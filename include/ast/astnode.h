/**
 * @file astnode.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-01-16
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#pragma once

#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "ast/variable.h"
#include "vm/vm.h"

namespace mocoder {

template <typename T>
using Ptr = std::shared_ptr<T>;

class ASTNode {
 public:
  virtual std::string GenJS() = 0;
  virtual std::string ExportASTAsJson() = delete;
  virtual void PrintTree(int depth) = 0;
  virtual void Eval(Ptr<Vars> v) = 0;
  virtual void GenVM(Ptr<Vars> v, std::vector<Op>& ops) = 0;
  virtual bool IsDef() { return false; };

  ASTNode() {}
  virtual ~ASTNode() {}
};
}  // namespace mocoder
