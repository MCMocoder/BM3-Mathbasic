/**
 * @file astnode.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-01-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ASTNODE_H_
#define ASTNODE_H_

#include <memory>
#include <string>
#include <unordered_set>
#include <list>
#include <iostream>

namespace mocoder {
class ASTNode {
public:

  virtual std::string GenJS()=0;
  virtual std::string ExportASTAsJson()=delete;
  virtual void PrintTree(int depth)=0;

  ASTNode(){}
  virtual ~ASTNode(){}
};
} // namespace mocoder

#endif
