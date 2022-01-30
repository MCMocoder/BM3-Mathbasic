/**
 * @file parse.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-01-22
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#ifndef PARSE_H_
#define PARSE_H_

#include "ast/nodes.h"
#include "lex/lexer.h"
#include <list>
#include <map>
#include <memory>
#include <string>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

namespace mocoder {
class Parser {
public:
  using TokenList = Lexer::TokenList;
  using Token = Lexer::Token;

  TokenList tokens_;
  TokenList::iterator iter_;
  Token curtok_;
  std::list<std::string> errors_;
  bool eof_ = false;
  long linenum_ = 0;
  std::list<std::unordered_set<std::string>> curscope_;

  void ConsumeToken() {
    ++iter_;
    if(iter_==tokens_.end()) {
      eof_=true;
      return;
    }
    curtok_=*iter_;
  }

  void EnterScope() {
    curscope_.push_front(std::unordered_set<std::string>());
  }

  void ExitScope() {
    curscope_.pop_front();
  }

  void AddVar(const std::string& varname) {
    curscope_.front().insert(varname);
  }

  Identifier* ParseIdentifier();
  Number* ParseNumber();
  ASTNode* ParseParenExpr();
  ASTNode* ParseIdExpr();
  ValexprList* ParseValExprList();
  IdentifierList* ParseIdentifierList();
  ASTNode* ParsePrimaryExpr();
  ASTNode* ParseAddExprTop();
  ASTNode* ParsePowExpr();
  ASTNode* ParseMulExpr();
  ASTNode* ParseAddExpr(ASTNode* lchild);
  Condexpr* ParseCondExpr();
  AssignStmt* ParseAssignStmt();
  InputStmt* ParseInputStmt();
  PrintStmt* ParsePrintStmt();
  IfStmt* ParseIfStmt();
  WhileStmt* ParseWhileStmt();
  DoLoopStmt* ParseDoLoopStmt();
  std::list<ASTNode*> ParseTop(); 
  std::shared_ptr<RootNode> Parse();


  Parser(TokenList &tokens) : tokens_(std::move(tokens)) {
    curtok_ = tokens_.front();
    iter_ = tokens_.begin();
    curscope_.push_front(std::unordered_set<std::string>());
  }
};

static const std::unordered_map<std::string, int /*ParamNum*/> funcmap =
    std::unordered_map<std::string, int>(
        {{"cos", 1}, {"sin", 1}, {"tan", 1}, {"lg", 1}, {"ln", 1}, {"log", 2}});

class Sema {
public:
  static bool SearchVar(const std::string &varname,
                        const Parser* psr) {
    for(const std::unordered_set<std::string>& scope:psr->curscope_) {
      if(scope.count(varname)!=0) {
        return true;
      }
    }
    return false;
  }
};

} // namespace mocoder

#endif
