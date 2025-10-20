#include "DragonLexer.hpp"
#include "Token.hpp"
#include <cctype>
#include <string>

DragonLexer::DragonLexer(const std::string &input)
  : Lexer(input) {}

Token DragonLexer::nextToken() {
  if (peek == EOF_CHAR) {
    return Token(TokenType::EOF_T, "EOF", line);
  }

  if (std::isspace(peek)) {
    return WS();
  }

  // TODO: finish me. You need to consider when to call ID()
  if (std::isalpha(peek) || peek == '_') {
    return ID();
  }
  
  if (std::isdigit(peek)) {
    return NUMBER();
  }

  int cur = peek;
  switch (cur) {
    // TODO: finish me. You need to consider EQ, NE, LT, LE, GT, GE
    case '(':
      advance(); return Token(TokenType::L_PAREN, "(", line);
    case ')':
      advance(); return Token(TokenType::R_PAREN, ")", line);
    case '{':
      advance(); return Token(TokenType::L_BRACE, "{", line);
    case '}':
      advance(); return Token(TokenType::R_BRACE, "}", line);
    case '[':
      advance(); return Token(TokenType::L_BRACKET, "[", line);
    case ']':
      advance(); return Token(TokenType::R_BRACKET, "]", line);
    case ',':
      advance(); return Token(TokenType::COMMA, ",", line);
    case ';':
      advance(); return Token(TokenType::SEMI, ";", line);
    case '+':
      advance(); return Token(TokenType::PLUS, "+", line);
    case '-':
      advance(); return Token(TokenType::MINUS, "-", line);
    case '*':
      advance(); return Token(TokenType::MUL, "*", line);
    case '/':
      advance(); return Token(TokenType::DIV, "/", line);
    case '.':
      advance(); return Token(TokenType::DOT, ".", line);
    case '\'':
      advance(); return Token(TokenType::SQUOTE, "\'", line);
    case '=' :
      advance(); return Token(TokenType::EQ, "=", line);
    case '<' :
      advance();
      if (peek == '=') {
        advance();
        return Token(TokenType::LE, "<=", line);
      } else if (peek == '>') {
        advance();
        return Token(TokenType::NE, "<>", line);
      } else {
        return Token(TokenType::LT, "<", line);
      }
    case '>' :
      advance();
      if (peek == '=') {
        advance();
        return Token(TokenType::GE, ">=", line);
      } else {
        return Token(TokenType::GT, ">", line);
      }
    default:
      char ch = static_cast<char>(peek);
      std::string s(1, ch);
      advance();
      return Token(TokenType::UNKNOWN, s, line);
  }
}

Token DragonLexer::WS() {
  // TODO: finish me. The WS() method should consume all whitespace characters (spaces, tabs, newlines)
  while(std::isspace(peek)) {
    advance();
  }
  return Token(TokenType::WS, " ", line);
}

Token DragonLexer::ID() {
  // TODO: finish me. The ID() method should consume an identifier or keyword.
  int startPos = pos;
  do {
    advance();
  } while (std::isalnum(peek) || peek == '_');
  std::string word = input.substr(startPos, pos - startPos);
  Token tk = kwTable.getKeyword(word);
  return Token(tk.type, tk.text, line);
}

bool DragonLexer::is_digits() {
  if (!std::isdigit(peek)) {
    return false;
  }
  while (true) {
    if (std::isdigit(peek)) {
      advance();
    } else if (peek == '\'') {
      advance();
      if (std::isdigit(peek)) {
        continue;
      } else {
        resetPos(pos - 1);
        break;
      }
    } else {
      break;
    }
  }
  return true;
}

Token DragonLexer::NUMBER() {
  // TODO: finish me. The NUMBER() method should consume an integer, real number, or scientific notation number.
  int startPos = pos; 
  is_digits();
  if (peek == '.') {
    advance();
    if (!is_digits()) {
      resetPos(pos - 1);
      std::string intNum = input.substr(startPos, pos - startPos);
      return Token(TokenType::INT, intNum, line);
    }
    if (peek == 'e' || peek == 'E') {
      bool hasPlusMinus = false;
      advance();
      if (peek == '+' || peek == '-') {
        advance();
        hasPlusMinus = true;
      }
      if (!is_digits()) {
        if (hasPlusMinus) {
          resetPos(pos - 2);
        } else {
          resetPos(pos - 1);
        }
        std::string realNum = input.substr(startPos, pos - startPos);
        return Token(TokenType::REAL, realNum, line);
      }
      std::string sciNum = input.substr(startPos, pos - startPos);
      return Token(TokenType::SCI, sciNum, line);
    } else {
      std::string realNum = input.substr(startPos, pos - startPos);
      return Token(TokenType::REAL, realNum, line);
    }
  } else if (peek == 'e' || peek == 'E') {
    advance();
    bool hasPlusMinus = false;
    if (peek == '+' || peek == '-') {
      advance();
      hasPlusMinus = true;
    }
    if (!is_digits()){
      if (hasPlusMinus) {
        resetPos(pos - 2);
      } else {
        resetPos(pos - 1);
      }
      std::string intNum = input.substr(startPos, pos - startPos);
      return Token(TokenType::INT, intNum, line);
    }
    std::string sciNum = input.substr(startPos, pos - startPos);
    return Token(TokenType::SCI, sciNum, line);
  } else {
    std::string intNum = input.substr(startPos, pos - startPos);
    return Token(TokenType::INT, intNum, line);
  }
}
