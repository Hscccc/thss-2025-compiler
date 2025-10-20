#pragma once
#include "Lexer.hpp"
#include "KeywordTable.hpp"

class DragonLexer : public Lexer {
public:
  DragonLexer(const std::string &input);
  Token nextToken() override;
  

private:
  KeywordTable kwTable;

  bool is_digits(); // helper for NUMBER() handles fragment DIGITS : [0-9] ('\''? [0-9])* 
  Token WS();
  Token ID();
  Token NUMBER(); // handles INT/REAL/SCI
};
