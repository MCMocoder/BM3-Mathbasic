/**
 * @file lexer.cc
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-01-16
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#include "lex/lexer.h"
#include <stdexcept>

namespace mocoder {

using Token = Lexer::Token;
using TokenList = Lexer::TokenList;

Token Lexer::LexIdentifier(const std::string &source) {
  std::string ident;
  ident.push_back(source[lexbegin]);
  for (int forward = lexbegin+1;; ++forward) {
    if (forward >= source.size()) {
      lexbegin = forward - 1;
      break;
    }
    char present = source[forward];
    if ((present >= 'a' && present <= 'z') ||
        (present >= 'A' && present <= 'Z') ||
        (present >= '0' && present <= '9')) {
      ident.push_back(present);
    } else if (!(ident == "LOOP")) {
      lexbegin = forward - 1;
      break;
    }
  }
  return Token(IDENTIFIER, ident);
}

Token Lexer::LexNumber(const std::string &source) {
  std::string num;
  num.push_back(source[lexbegin]);
  for (int forward = lexbegin + 1;; ++forward) {
    if (forward >= source.size()) {
      lexbegin = forward - 1;
      break;
    }
    char present = source[forward];
    if ((present >= '0' && present <= '9') || present == '.') {
      num.push_back(present);
    } else {
      lexbegin = forward - 1;
      break;
    }
  }
  return Token(NUMBER, num);
}

Token Lexer::LexOper(const std::string &source) {
  std::string oper;
  oper.push_back(source[lexbegin]);
  for (int forward = lexbegin + 1;; ++forward) {
    if (forward >= source.size()) {
      lexbegin = forward - 1;
      break;
    }
    char present = source[forward];
    if (present == '=' || present == '>') {
      oper.push_back(present);
    } else {
      lexbegin = forward - 1;
      break;
    }
  }
  if (oper == ">=") {
    return Token(GREATEREQUAL, "");
  }
  if (oper == ">") {
    return Token(GREATER, "");
  }
  if (oper == "<=") {
    return Token(LESSEQUAL, "");
  }
  if (oper == "<") {
    return Token(LESS, "");
  }
  if (oper == "<>") {
    return Token(NOEQUAL, "");
  }
  if (oper == "==") {
    return Token(EQUAL, "");
  }
  if (oper == "=") {
    return Token(ASSIGN, "");
  }
  if (oper == "+") {
    return Token(ADD, "");
  }
  if (oper == "-") {
    return Token(SUB, "");
  }
  if (oper == "*") {
    return Token(MUL, "");
  }
  if (oper == "/") {
    return Token(DIV, "");
  }
  if (oper == "^") {
    return Token(POW, "");
  } else {
    std::__throw_logic_error("Unexpected Token");
  }
}

TokenList Lexer::Lex(const std::string &source) {
  TokenList result;
  for (; lexbegin < source.size(); ++lexbegin) {
    char beginchar = source[lexbegin];
    if ((beginchar >= 'a' && beginchar <= 'z') ||
        (beginchar >= 'A' && beginchar <= 'Z')) {
      result.push_back(LexIdentifier(source));
    } else if (beginchar >= '0' && beginchar <= '9') {
      result.push_back(LexNumber(source));
    } else if (beginchar == '>' || beginchar == '<' || beginchar == '=' ||
               beginchar == '+' || beginchar == '-' || beginchar == '*' ||
               beginchar == '/' || beginchar == '^') {
      result.push_back(LexOper(source));
    } else {
      switch (beginchar) {
      case '(':
        result.push_back(Token(LBRACKET, ""));
        break;
      case ')':
        result.push_back(Token(RBRACKET, ""));
        break;
      case ',':
        result.push_back(Token(COMMA, ""));
        break;
      case '\n':
        result.push_back(Token(NEWLINE, ""));
        break;
      case ' ':
      case '\t':
        continue;
      default:
        std::__throw_logic_error("Unexpected Token");
      }
    }
  }
  return result;
}
} // namespace mocoder
