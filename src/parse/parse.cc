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
#include "ast/astnode.h"
#include "ast/rootnode.h"
#include "ast/statement/whilestmt.h"
#include <list>
#include <memory>
#include <stdexcept>
#include <string>

namespace mocoder {

using TokenList = Lexer::TokenList;
using Token = Lexer::Token;

Identifier *Parser::ParseIdentifier() {
  Identifier *result = new Identifier(curtok_.second);
  ConsumeToken();

  if (!Sema::SearchVar(result->name_, this)) {
    AddVar(result->name_);
  }

  return result;
}

Number *Parser::ParseNumber() {
  Number *result = new Number(std::stod(curtok_.second));
  ConsumeToken();
  return result;
}

ASTNode *Parser::ParseParenExpr() {
  ConsumeToken(); // LBRACKET
  ASTNode *result = ParseAddExprTop();
  if (curtok_.first == Lexer::LBRACKET) {
    ConsumeToken(); // RBRACKET
    return result;
  } else {
    delete result;
    std::__throw_logic_error("Expected ')'");
  }
}

ASTNode *Parser::ParseIdExpr() {
  std::string id = curtok_.second;
  ConsumeToken(); // Identifier
  if (curtok_.first == Lexer::LBRACKET) {
    // FuncCall
    ConsumeToken(); // LBRACKET

    if (funcmap.count(id) == 0) {
      std::__throw_logic_error("Use of undeclared function");
    }

    CallFuncExpr *result = new CallFuncExpr(id, ParseValExprList());

    if (result->params_->exprs_.size() != funcmap.find(id)->second) {
      delete result;
      std::__throw_logic_error("Unexpected Param Number");
    }

    if (curtok_.first != Lexer::RBRACKET) {
      delete result;
      std::__throw_logic_error("Expected ')'");
    }
    ConsumeToken(); // RBRACKET
    return result;
  } else {
    // Variable(Identifier)
    Identifier *result = new Identifier(id);

    if (!Sema::SearchVar(result->name_, this)) {
      delete result;
      std::__throw_logic_error("Use of undeclared variable");
    }

    return result;
  }
}

ValexprList *Parser::ParseValExprList() {
  ValexprList *result = new ValexprList();
  while (true) {
    result->AddExpr(ParseAddExprTop());
    if (curtok_.first != Lexer::COMMA) {
      return result;
    }
    ConsumeToken(); // COMMA
    if (eof_) {
      delete result;
      std::__throw_logic_error("Unexpected EOF");
    }
  }
}

IdentifierList *Parser::ParseIdentifierList() {
  IdentifierList *result = new IdentifierList();
  while (true) {
    Identifier *curid = new Identifier(curtok_.second);
    ConsumeToken(); // IDENTIFIER

    if (!Sema::SearchVar(curid->name_, this)) {
      AddVar(curid->name_);
    }

    result->AddId(curid);
    if (curtok_.first != Lexer::COMMA) {
      return result;
    }
    ConsumeToken(); // COMMA
    if (eof_) {
      delete result;
      delete curid;
      std::__throw_logic_error("Unexpected EOF");
    }
  }
}

ASTNode *Parser::ParsePrimaryExpr() {
  switch (curtok_.first) {
  case Lexer::IDENTIFIER:
    return ParseIdExpr();
  case Lexer::NUMBER:
    return ParseNumber();
  case Lexer::LBRACKET:
    return ParseParenExpr();
  default:
    std::__throw_logic_error("Unexpected Expression");
  }
}

ASTNode *Parser::ParseAddExprTop() {
  if (curtok_.first == Lexer::ADD || curtok_.first == Lexer::SUB) {
    return ParseAddExpr(nullptr);
  }
  ASTNode *lchild = ParseMulExpr();
  return ParseAddExpr(lchild);
}

ASTNode *Parser::ParseAddExpr(ASTNode *lchild) {
  while (true) {
    if (!(curtok_.first == Lexer::ADD || curtok_.first == Lexer::SUB)) {
      return lchild;
    }
    Token oper = curtok_;
    ConsumeToken(); // ADD or SUB
    ASTNode *rchild = ParseMulExpr();
    lchild = new Valexpr(oper.first, lchild, rchild);
  }
}

ASTNode *Parser::ParseMulExpr() {
  ASTNode *lchild = ParsePowExpr();
  while (true) {
    if (!(curtok_.first == Lexer::MUL || curtok_.first == Lexer::DIV)) {
      return lchild;
    }
    Token oper = curtok_;
    ConsumeToken(); // MUL or DIV
    ASTNode *rchild = ParsePowExpr();
    lchild = new Valexpr(oper.first, lchild, rchild);
  }
}

ASTNode *Parser::ParsePowExpr() {
  ASTNode *lchild = ParsePrimaryExpr();
  if (curtok_.first != Lexer::POW) {
    return lchild;
  }
  ConsumeToken(); // POW
  ASTNode *rchild = ParsePowExpr();
  ASTNode *result = new Valexpr(Lexer::POW, lchild, rchild);
  return result;
}

Condexpr *Parser::ParseCondExpr() {
  ASTNode *lchild = ParseAddExprTop();
  if (!(curtok_.first >= Lexer::GREATEREQUAL &&
        curtok_.first <= Lexer::EQUAL)) {
    delete lchild;
    std::__throw_logic_error("Expected Relop");
  }
  int relop = curtok_.first;
  ConsumeToken(); // (Relop)
  ASTNode *rchild = ParseAddExprTop();
  Condexpr *result = new Condexpr(relop, lchild, rchild);
  return result;
}

AssignStmt *Parser::ParseAssignStmt() {
  Identifier *var = ParseIdentifier();
  if (curtok_.first != Lexer::ASSIGN) {
    delete var;
    std::__throw_logic_error("Expected ASSIGN");
  }
  ConsumeToken(); // ASSIGN
  ASTNode *expr = ParseAddExprTop();
  AssignStmt *result = new AssignStmt(var, expr);
  return result;
}

InputStmt *Parser::ParseInputStmt() {
  ConsumeToken(); // INPUT
  IdentifierList *vars = ParseIdentifierList();
  return new InputStmt(vars);
}

PrintStmt *Parser::ParsePrintStmt() {
  ConsumeToken(); // PRINT
  ValexprList *list = ParseValExprList();
  return new PrintStmt(list);
}

IfStmt *Parser::ParseIfStmt() {
  ConsumeToken(); // IF
  Condexpr *cond = ParseCondExpr();
  if (curtok_.second != "THEN") {
    delete cond;
    std::__throw_logic_error("Expected THEN");
  }
  ConsumeToken(); // THEN
  if (curtok_.first != Lexer::NEWLINE) {
    delete cond;
    std::__throw_logic_error("Expected NEWLINE");
  }
  ConsumeToken(); // NEWLINE
  ++linenum_;
  EnterScope();
  std::list<ASTNode *> stmts = ParseTop();
  IfStmt *result = new IfStmt(cond, stmts);
  result->declvars_ = curscope_.front();
  if (curtok_.second == "ELSE") {
    ConsumeToken(); // ELSE
    ExitScope();
    EnterScope();
    std::list<ASTNode *> elsestmts = ParseTop();
    ElseStmt *elseres = new ElseStmt(elsestmts);
    elseres->declvars_ = curscope_.front();
    result->else_=elseres;
  }
  ExitScope();
  return result;
}

WhileStmt *Parser::ParseWhileStmt() {
  ConsumeToken(); // WHILE
  Condexpr *cond = ParseCondExpr();
  if (curtok_.first != Lexer::NEWLINE) {
    delete cond;
    std::__throw_logic_error("Expected NEWLINE");
  }
  ConsumeToken(); // NEWLINE
  ++linenum_;
  EnterScope();
  std::list<ASTNode *> stmts = ParseTop();
  WhileStmt *result = new WhileStmt(cond, stmts);
  if (curtok_.second != "WEND") {
    delete result;
    std::__throw_logic_error("Expected WEND");
  }
  ConsumeToken(); // WEND
  result->declvars_ = curscope_.front();
  ExitScope();
  return result;
}

DoLoopStmt *Parser::ParseDoLoopStmt() {
  ConsumeToken(); // DO
  if (curtok_.first != Lexer::NEWLINE) {
    std::__throw_logic_error("Expected NEWLINE");
  }
  ConsumeToken(); // NEWLINE
  ++linenum_;
  EnterScope();
  std::list<ASTNode *> stmts = ParseTop();
  if (curtok_.second != "LOOPUNTIL") {
    std::__throw_logic_error("Expected LOOP UNTIL");
  }
  ConsumeToken(); // DOLOOP
  Condexpr *cond = ParseCondExpr();
  DoLoopStmt *result = new DoLoopStmt(cond, stmts);
  result->declvars_ = curscope_.front();
  ExitScope();
  return result;
}

std::list<ASTNode *> Parser::ParseTop() {
  std::list<ASTNode *> result;
  while (!eof_ || curtok_.second == "END"||curtok_.first==Lexer::NEWLINE) {
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
      } else if (curtok_.second == "END"||eof_) {
        if(curtok_.second!="END") {
          std::__throw_logic_error("Expected END");
        }
        ConsumeToken(); // END
        if(curtok_.second=="IF") {
          ConsumeToken();
        }
        return result;
      } else if (curtok_.second == "WEND" || curtok_.second == "LOOPUNTIL" ||
                 curtok_.second == "ELSE") {
        return result;
      } else if (curtok_.first == Lexer::NEWLINE) {
        ConsumeToken();
        ++linenum_;
        continue;
      } else {
        result.push_back(ParseAssignStmt());
      }
    } catch (std::logic_error& error) {
      std::cout << error.what() << std::endl;
      
      /*while (curtok_.first != Lexer::NEWLINE || !eof_) {
        ConsumeToken();
      }
      ConsumeToken();*/
      return result;
    }
  }
  std::__throw_logic_error("Unexpected EOF");
}

std::shared_ptr<RootNode> Parser::Parse() {
  std::list<ASTNode *> stmts = ParseTop();
  std::shared_ptr<RootNode> node(new RootNode(stmts));
  node->declvars_ = curscope_.front();
  return node;
}

} // namespace mocoder
