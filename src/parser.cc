/*
 * Copyright (C) Rida Bazzi, 2020
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */
#include <iostream>
#include <cstdlib>
#include "../include/parser.h"
#include "../include/symbol_table.h"

using namespace std;

void Parser::syntax_error()
{
    cout << "SYNTAX ERROR\n";
    exit(1);
}

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

// Parsing

// This function is simply to illustrate the GetToken() function
// you will not need it for your project and you can delete it
// the function also illustrates the use of peek()
void Parser::ConsumeAllInput()
{
    Token token;
    int i = 1;
    
    token = lexer.peek(i);
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        i = i+1;
        token = lexer.peek(i);
        token.Print();
    }
}

/**
 * @brief Parse program input
 * 
 */
void Parser::parse_input()
{
    parse_tokens_section();
    Token input_text = expect(INPUT_TEXT);
}
/**
 * @brief Parse the tokens section
 * 
 */
void Parser::parse_tokens_section()
{
    parse_token_list();
    expect(HASH);
}

/**
 * @brief Parse list of tokens
 * 
 */
void Parser::parse_token_list()
{
    parse_token();
    Token t = lexer.peek(1);
    if(t.token_type == COMMA)
    {
        expect(COMMA);
        parse_token_list();
    }
}

/**
 * @brief Parse a token
 * 
 */
void Parser::parse_token()
{
    Token id = expect(ID);
    REG * expr = parse_expr();
    token_table.push(id.lexeme,expr);
}

/**
 * @brief Parse an expression
 * 
 * @return REG* 
 */
REG * Parser::parse_expr()
{
    REG * reg = new REG();//!< expression to be returned

    Token t = lexer.GetToken();
    if(t.token_type == CHAR)
    {
        State * neighbor1 = new State();
        reg->start->first_neighbor = neighbor1;//!< set transition state
        reg->start->first_label = t.lexeme[0];//!< set the transion label
        reg->final = neighbor1;//!< make new state final
    }
    else if(t.token_type == LPAREN)
    {
        reg->start->first_neighbor = parse_expr()->start; //!< parse first expression
        expect(RPAREN);//!< consume/check for right paren

        Token tt = lexer.GetToken(); //!< consume manditory operator
        if(tt.token_type == DOT) //!< concatanation
        {
            
        }
        else if(tt.token_type == OR) //!< union
        {

        }
        else if(tt.token_type == STAR) //!< kleene star
        {

        }
        else{syntax_error();}
    }
    else if(t.token_type == UNDERSCORE) //!< epsilon
    {
        //TODO
    }
    else{syntax_error();}
    //TOTO: concatanation
    return reg;
}

int main()
{
    Parser parser;
    parser.parse_input();

}