/*
 * Copyright (C) Rida Bazzi, 2019
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"
#include "REG.h"
#include "symbol_table.h"

class Parser {
  public:

    void ConsumeAllInput();
    void parse_input();
    void parse_tokens_section();
    void parse_token_list();
    void parse_token();
    REG * parse_expr();
    Token expect(TokenType expected_type);


  private:
    LexicalAnalyzer lexer;
    void syntax_error();
   
};

#endif

