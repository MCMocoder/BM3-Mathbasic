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

#ifndef ASTNODE_H_
#define ASTNODE_H_

//#define DEBUG_MODE

#include <memory>
#include <string>
#include <unordered_set>
#include <list>
#include <iostream>

namespace mocoder {

template<typename T>
using Ptr=std::shared_ptr<T>;

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
