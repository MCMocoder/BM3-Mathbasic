/**
 * @file lexer.h
 * @author MCMocoder (mcmocoder@mocoder.xyz)
 * @brief
 * @version 0.1
 * @date 2022-01-16
 *
 * @copyright Copyright (c) 2022 Mocoder Studio
 *
 */

#ifndef LEXER_H_
#define LEXER_H_

#include <list>
#include <map>
#include <stdexcept>
#include <string>

namespace mocoder {
class Lexer {
public:
  enum TokenType {
    IDENTIFIER,
    NUMBER,
    GREATEREQUAL,
    GREATER,
    LESSEQUAL,
    LESS,
    NOEQUAL,
    EQUAL,
    ASSIGN,
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    LBRACKET,
    RBRACKET,
    COMMA,
    NEWLINE
  };

  typedef std::list<std::pair<TokenType, std::string>> TokenList;
  typedef std::pair<TokenType, std::string> Token;
  int linenum_ = 1;
  bool success_=true;
  
  int lexbegin = 0;

  Token LexIdentifier(const std::string &source);
  Token LexNumber(const std::string &source);
  Token LexOper(const std::string &source);
  TokenList Lex(const std::string &source);
};

} // namespace mocoder

#endif
