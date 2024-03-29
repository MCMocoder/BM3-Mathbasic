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

#pragma once

#include <exception>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <ostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "ast/nodes.h"
#include "ast/statement/defstmt.h"
#include "ast/statement/doloopstmt.h"
#include "ast/statement/ifstmt.h"
#include "lex/lexer.h"

namespace mocoder {

inline std::string ToLowerCase(const std::string &str) {
  std::string result;
  for (int i = 0; i < str.size(); ++i) {
    if (str[i] >= 97) {
      result.push_back(str[i]);
    } else {
      result.push_back(str[i] + 0x20);
    }
  }
  return result;
}

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
  bool success_ = true;

  void ConsumeToken() {
    ++iter_;
    if (iter_ == tokens_.end()) {
      eof_ = true;
      return;
    }
    curtok_ = *iter_;
  }

  void EnterScope() { curscope_.push_front(std::unordered_set<std::string>()); }

  void ExitScope() { curscope_.pop_front(); }

  void AddVar(const std::string &varname) { curscope_.front().insert(varname); }

  Ptr<Identifier> ParseIdentifier();
  Ptr<Number> ParseNumber();
  Ptr<Valexpr> ParseParenExpr();
  Ptr<Valexpr> ParseIdExpr();
  Ptr<ValexprList> ParseValExprList();
  std::list<Ptr<Identifier>> ParseIdentifierList();
  Ptr<Valexpr> ParsePrimaryExpr();
  Ptr<Valexpr> ParseAddExprTop();
  Ptr<Valexpr> ParsePowExpr();
  Ptr<Valexpr> ParseMulExpr();
  Ptr<Valexpr> ParseAddExpr(Ptr<Valexpr> lchild);
  Ptr<Condexpr> ParseCondExpr();
  Ptr<AssignStmt> ParseAssignStmt();
  Ptr<InputStmt> ParseInputStmt();
  Ptr<PrintStmt> ParsePrintStmt();
  Ptr<IfStmt> ParseIfStmt();
  Ptr<WhileStmt> ParseWhileStmt();
  Ptr<DoLoopStmt> ParseDoLoopStmt();
  Ptr<DefStmt> ParseDefStmt();
  Ptr<CallFuncExpr> ParseCallFuncExpr(std::string &id);
  Ptr<RetStmt> ParseRetStmt();
  std::list<Ptr<ASTNode>> ParseTop();
  std::shared_ptr<RootNode> Parse();

  Parser(TokenList &tokens) : tokens_(std::move(tokens)) {
    curtok_ = tokens_.front();
    iter_ = tokens_.begin();
    curscope_.push_front(std::unordered_set<std::string>());
  }
};

static std::unordered_map<std::string, int /*ParamNum*/> funcmap =
    std::unordered_map<std::string, int>(
        {{"cos", 1}, {"sin", 1}, {"tan", 1}, {"lg", 1}, {"ln", 1}, {"log", 2}});

class Sema {
 public:
  static bool SearchVar(const std::string &varname, const Parser *psr,
                        int linenum) {
    for (const std::unordered_set<std::string> &scope : psr->curscope_) {
      if (scope.count(varname) != 0) {
        if (ToLowerCase(varname) == "paimon") {
          std::cout << "Warning at Line " << linenum + 1 << ":"
                    << "Ehe Te Nandayo" << std::endl;
        }
        return true;
      }
    }
    return false;
  }
};

}  // namespace mocoder
