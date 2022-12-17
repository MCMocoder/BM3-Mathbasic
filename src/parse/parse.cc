/**
 * @file parse.cc
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-01-22
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#include "parse/parse.h"

#include <list>
#include <memory>
#include <stdexcept>
#include <string>

#include "ast/astnode.h"
#include "ast/expression/valexpr.h"
#include "ast/rootnode.h"
#include "ast/statement/whilestmt.h"

namespace mocoder {

using TokenList = Lexer::TokenList;
using Token = Lexer::Token;

Ptr<Identifier> Parser::ParseIdentifier() {
  Ptr<Identifier> result(new Identifier(curtok_.second));
  ConsumeToken();

  if (!Sema::SearchVar(result->name_, this)) {
    AddVar(result->name_);
  }

  return result;
}

Ptr<Number> Parser::ParseNumber() {
  Ptr<Number> result(new Number(std::stod(curtok_.second)));
  ConsumeToken();
  return result;
}

Ptr<Valexpr> Parser::ParseParenExpr() {
  ConsumeToken();  // LBRACKET
  Ptr<Valexpr> result = ParseAddExprTop();
  if (curtok_.first == Lexer::RBRACKET) {
    ConsumeToken();  // RBRACKET
    return result;
  } else {
    std::__throw_logic_error("Expected ')'");
  }
}

Ptr<CallFuncExpr> Parser::ParseCallFuncExpr(std::string& id) {
  ConsumeToken();  // LBRACKET

  if (funcmap.count(id) == 0) {
    std::__throw_logic_error("Use of undeclared function");
  }

  Ptr<CallFuncExpr> result(new CallFuncExpr(id, ParseValExprList()));

  if (result->params_->exprs_.size() != funcmap.find(id)->second) {
    std::__throw_logic_error("Unexpected Param Number");
  }

  if (curtok_.first != Lexer::RBRACKET) {
    std::__throw_logic_error("Expected ')'");
  }
  ConsumeToken();  // RBRACKET
  return result;
}

Ptr<Valexpr> Parser::ParseIdExpr() {
  std::string id = curtok_.second;
  ConsumeToken();  // Identifier
  if (curtok_.first == Lexer::LBRACKET) {
    // FuncCall
    return ParseCallFuncExpr(id);
  } else {
    // Variable(Identifier)
    Ptr<Identifier> result(new Identifier(id));

    if (!Sema::SearchVar(result->name_, this)) {
      std::__throw_logic_error(
          (std::string("Use of undeclared variable ") + result->name_).c_str());
    }

    return result;
  }
}

Ptr<ValexprList> Parser::ParseValExprList() {
  Ptr<ValexprList> result(new ValexprList());
  while (true) {
    result->AddExpr(ParseAddExprTop());
    if (curtok_.first != Lexer::COMMA) {
      return result;
    }
    ConsumeToken();  // COMMA
    if (eof_) {
      std::__throw_logic_error("Unexpected EOF");
    }
  }
}

std::list<Ptr<Identifier>> Parser::ParseIdentifierList() {
  std::list<Ptr<Identifier>> result;
  while (true) {
    Ptr<Identifier> curid(new Identifier(curtok_.second));
    ConsumeToken();  // IDENTIFIER

    if (!Sema::SearchVar(curid->name_, this)) {
      AddVar(curid->name_);
    }

    result.push_back(curid);
    if (curtok_.first != Lexer::COMMA) {
      return result;
    }
    ConsumeToken();  // COMMA
    if (eof_) {
      std::__throw_logic_error("Unexpected EOF");
    }
  }
}

Ptr<Valexpr> Parser::ParsePrimaryExpr() {
  switch (curtok_.first) {
    case Lexer::IDENTIFIER:
      return ParseIdExpr();
    case Lexer::NUMBER:
      return ParseNumber();
    case Lexer::LBRACKET:
      return ParseParenExpr();
    default:
      return nullptr;
  }
}

Ptr<Valexpr> Parser::ParseAddExprTop() {
  if (curtok_.first == Lexer::ADD || curtok_.first == Lexer::SUB) {
    return ParseAddExpr(nullptr);
  }
  Ptr<Valexpr> lchild = ParseMulExpr();
  return ParseAddExpr(lchild);
}

Ptr<Valexpr> Parser::ParseAddExpr(Ptr<Valexpr> lchild) {
  while (true) {
    if (!(curtok_.first == Lexer::ADD || curtok_.first == Lexer::SUB)) {
      return lchild;
    }
    Token oper = curtok_;
    ConsumeToken();  // ADD or SUB
    Ptr<Valexpr> rchild = ParseMulExpr();
    lchild = std::shared_ptr<Valexpr>(new Valexpr(oper.first, lchild, rchild));
  }
}

Ptr<Valexpr> Parser::ParseMulExpr() {
  Ptr<Valexpr> lchild = ParsePowExpr();
  while (true) {
    if (!(curtok_.first == Lexer::MUL || curtok_.first == Lexer::DIV)) {
      return lchild;
    }
    Token oper = curtok_;
    ConsumeToken();  // MUL or DIV
    Ptr<Valexpr> rchild = ParsePowExpr();
    lchild = std::shared_ptr<Valexpr>(new Valexpr(oper.first, lchild, rchild));
  }
}

Ptr<Valexpr> Parser::ParsePowExpr() {
  Ptr<Valexpr> lchild = ParsePrimaryExpr();
  if (curtok_.first != Lexer::POW) {
    return lchild;
  }
  ConsumeToken();  // POW
  Ptr<Valexpr> rchild = ParsePowExpr();
  Ptr<Valexpr> result(new Valexpr(Lexer::POW, lchild, rchild));
  return result;
}

Ptr<Condexpr> Parser::ParseCondExpr() {
  Ptr<Valexpr> lchild = ParseAddExprTop();
  if (!(curtok_.first >= Lexer::GREATEREQUAL &&
        curtok_.first <= Lexer::EQUAL)) {
    std::__throw_logic_error("Expected Relop");
  }
  int relop = curtok_.first;
  ConsumeToken();  // (Relop)
  Ptr<Valexpr> rchild = ParseAddExprTop();
  Ptr<Condexpr> result(new Condexpr(relop, lchild, rchild));
  return result;
}

Ptr<AssignStmt> Parser::ParseAssignStmt() {
  Ptr<Identifier> var = ParseIdentifier();
  if (curtok_.first != Lexer::ASSIGN) {
    std::__throw_logic_error("Expected ASSIGN");
  }
  ConsumeToken();  // ASSIGN
  Ptr<Valexpr> expr = ParseAddExprTop();
  Ptr<AssignStmt> result(new AssignStmt(var, expr));
  return result;
}

Ptr<InputStmt> Parser::ParseInputStmt() {
  ConsumeToken();  // INPUT
  Ptr<InputStmt> res(new InputStmt());
  std::list<Ptr<Identifier>> ids = ParseIdentifierList();
  res->vars_ = ids;
  return res;
}

Ptr<PrintStmt> Parser::ParsePrintStmt() {
  ConsumeToken();  // PRINT
  Ptr<ValexprList> list = ParseValExprList();
  return Ptr<PrintStmt>(new PrintStmt(list));
}

Ptr<IfStmt> Parser::ParseIfStmt() {
  ConsumeToken();  // IF
  Ptr<Condexpr> cond = ParseCondExpr();
  if (curtok_.second != "THEN") {
    std::__throw_logic_error("Expected THEN");
  }
  ConsumeToken();  // THEN
  if (curtok_.first != Lexer::NEWLINE) {
    std::__throw_logic_error("Expected NEWLINE");
  }
  ConsumeToken();  // NEWLINE
  ++linenum_;
  EnterScope();
  std::list<Ptr<ASTNode>> stmts = ParseTop();
  Ptr<IfStmt> result(new IfStmt(cond, stmts));
  result->declvars_ = curscope_.front();
  if (curtok_.second == "ELSE") {
    ConsumeToken();  // ELSE
    ExitScope();
    EnterScope();
    std::list<Ptr<ASTNode>> elsestmts = ParseTop();
    Ptr<ElseStmt> elseres(new ElseStmt(elsestmts));
    elseres->declvars_ = curscope_.front();
    result->else_ = elseres;
  }
  ExitScope();
  return result;
}

Ptr<WhileStmt> Parser::ParseWhileStmt() {
  ConsumeToken();  // WHILE
  Ptr<Condexpr> cond = ParseCondExpr();
  if (curtok_.first != Lexer::NEWLINE) {
    std::__throw_logic_error("Expected NEWLINE");
  }
  ConsumeToken();  // NEWLINE
  ++linenum_;
  EnterScope();
  std::list<Ptr<ASTNode>> stmts = ParseTop();
  Ptr<WhileStmt> result(new WhileStmt(cond, stmts));
  if (curtok_.second != "WEND") {
    std::__throw_logic_error("Expected WEND");
  }
  ConsumeToken();  // WEND
  result->declvars_ = curscope_.front();
  ExitScope();
  return result;
}

Ptr<DoLoopStmt> Parser::ParseDoLoopStmt() {
  ConsumeToken();  // DO
  if (curtok_.first != Lexer::NEWLINE) {
    std::__throw_logic_error("Expected NEWLINE");
  }
  ConsumeToken();  // NEWLINE
  ++linenum_;
  EnterScope();
  std::list<Ptr<ASTNode>> stmts = ParseTop();
  if (curtok_.second != "LOOPUNTIL") {
    std::__throw_logic_error("Expected LOOP UNTIL");
  }
  ConsumeToken();  // DOLOOP
  Ptr<Condexpr> cond = ParseCondExpr();
  Ptr<DoLoopStmt> result(new DoLoopStmt(cond, stmts));
  result->declvars_ = curscope_.front();
  ExitScope();
  return result;
}

Ptr<RetStmt> Parser::ParseRetStmt() {
  ConsumeToken();  // RET
  return Ptr<RetStmt>(new RetStmt(ParseAddExprTop()));
}

Ptr<DefStmt> Parser::ParseDefStmt() {
  ConsumeToken();  // DEF
  if (curtok_.first != Lexer::IDENTIFIER) {
    std::__throw_logic_error("Expected IDENTIFIER(Function name)");
  }
  std::string name = curtok_.second;
  ConsumeToken();  // IDENTIFIER
  if (curtok_.first != Lexer::LBRACKET) {
    std::__throw_logic_error("Expected (");
  }
  ConsumeToken();  // LBRACKET
  EnterScope();
  std::list<Ptr<Identifier>> ids = ParseIdentifierList();
  if (curtok_.first != Lexer::RBRACKET) {
    std::__throw_logic_error("Expected )");
  }
  ConsumeToken();  // RBRACKET
  std::list<Ptr<ASTNode>> nodes = ParseTop();
  if (curtok_.second != "ENDDEF") {
    std::__throw_logic_error("Expected ENDDEF");
  }
  ConsumeToken();  // ENDDEF
  Ptr<DefStmt> result(new DefStmt(nodes, name, ids));
  result->declvars_ = curscope_.front();
  ExitScope();
  funcmap.emplace(name,ids.size());
  return result;
}

std::list<Ptr<ASTNode>> Parser::ParseTop() {
  std::list<Ptr<ASTNode>> result;
  while (!eof_ || curtok_.second == "END" || curtok_.first == Lexer::NEWLINE) {
    try {
      if (curtok_.second == "IF") {
        result.push_back(ParseIfStmt());
      } else if (curtok_.second == "WHILE") {
        result.push_back(ParseWhileStmt());
      } else if (curtok_.second == "DO") {
        result.push_back(ParseDoLoopStmt());
      } else if (curtok_.second == "INPUT") {
        result.push_back(ParseInputStmt());
      } else if (curtok_.second == "PRINT") {
        result.push_back(ParsePrintStmt());
      } else if (curtok_.second == "DEF") {
        result.push_back(ParseDefStmt());
      } else if (curtok_.second == "RET") {
        result.push_back(ParseRetStmt());
      } else if (curtok_.second == "END" || eof_) {
        if (curtok_.second != "END") {
          std::__throw_logic_error("Expected END");
        }
        ConsumeToken();  // END
        if (curtok_.second == "IF") {
          ConsumeToken();
        }
        return result;
      } else if (curtok_.second == "WEND" || curtok_.second == "LOOPUNTIL" ||
                 curtok_.second == "ELSE" || curtok_.second == "ENDDEF") {
        return result;
      } else if (curtok_.first == Lexer::NEWLINE) {
        ConsumeToken();
        ++linenum_;
        continue;
      } else {
        result.push_back(ParseAssignStmt());
      }
    } catch (std::logic_error& error) {
      std::cout << "Line " << linenum_ + 1 << " : " << error.what()
                << std::endl;

      while (curtok_.first != Lexer::NEWLINE && !eof_) {
        ConsumeToken();
      }
      ConsumeToken();  // NEWLINE
      success_ = false;
    }
  }
  std::__throw_logic_error("Unexpected EOF");
}

std::shared_ptr<RootNode> Parser::Parse() {
  std::list<Ptr<ASTNode>> stmts = ParseTop();
  EnterScope();
  Ptr<RootNode> node(new RootNode(stmts));
  node->declvars_ = curscope_.front();
  ExitScope();
  return node;
}

}  // namespace mocoder
