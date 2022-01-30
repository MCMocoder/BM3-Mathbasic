/**
 * @file identifierlist.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief 
 * @version 0.1
 * @date 2022-01-22
 * 
 * @copyright Copyright (c) 2022 Mocoder Studio
 * 
 */

#ifndef IDENTIFIERLIST_H_
#define IDENTIFIERLIST_H_

#include "ast/astnode.h"
#include "ast/identifier/identifier.h"
#include "lex/lexer.h"
#include <list>

namespace mocoder {
class IdentifierList : public ASTNode {
public:
  std::list<Identifier*> ids_;
  IdentifierList(){}
  virtual ~IdentifierList() {
    for(Identifier* id:ids_) {
      delete id;
    }
  }
  void AddId(Identifier* id) {
    ids_.push_back(id);
  }
  virtual void PrintTree(int depth) override {
    for(int i=0;i<depth;++i) {
      std::cout<<" ";
    }
    std::cout<<depth<<":"<<"IdentifierList";
    std::cout<<std::endl;
    for(ASTNode* stmt:ids_) {
      stmt->PrintTree(depth+1);
    }
  }
  // Do not need GenJS(), because it is only used in InputStmt
  virtual std::string GenJS() override {
    return "";
  }
};
} // namespace mocoder

#endif


