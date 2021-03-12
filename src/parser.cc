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
#include <sstream>
#include <string.h>
#include <vector>
#include <bits/stdc++.h> 

using namespace std;

//helper
void vectorCat(vector<State *>& v1, vector<State *>& v2)
{
    
}

void getChar(string& str, int& pos, char& dest)
{
    dest = str[pos];
    pos++;
}

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
    parse_tokens_section();//!< Generate REG graphs
    Token input_text = expect(INPUT_TEXT);//!< Consume input text
    
    // Tokenizing the input text
    vector<string> input_tokens;
    vector<string> tok_types; 
    stringstream s(input_text.lexeme);
    string buff;
    s>>buff;
    while(s >> buff){input_tokens.push_back(buff);}
    input_tokens.pop_back();

    for(int i = 0 ;i < token_table.size; i++)//!< iterate over the REG graphs
    {   
        Symbol * symbol = token_table.getIndex(i);
        REG * reg = symbol->expr;

        for(int j = 0;j < input_tokens.size(); j++)//!< loop over the input strings
        {
            string tok = input_tokens[j];
            char p;
            
            std::vector<State *> reachable;
            std::vector<State *> reachable_ = reg->epsilonClosure();

            for(int k = 0; k < tok.length(); k++)//!< loop over characters in the input strings
            {
                p = tok[k]; //!< consume character to process
                vector<State *> reachable_ = reg->start->reachableBy(p);
                reachable.insert(reachable.end(), reachable_.begin(), reachable_.end());

            }
        }
    }
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
        reg->start = new State();//!< initialize start state
        State * final = new State();//!< initilize final state
        reg->start->first_neighbor = final;//!< set transition state
        reg->start->first_label = t.lexeme[0];//!< set the transion label
        reg->final = final;//!< make new state final
    }
    else if(t.token_type == LPAREN)
    {
        REG * expression1 = parse_expr(); //!< pointer to parsed first expression
        expect(RPAREN);//!< consume/check for right paren

        Token tt = lexer.GetToken(); //!< consume manditory operator
        if(tt.token_type == END_OF_FILE)
        {
            
        }
        else if(tt.token_type == DOT) //!< concatanation
        {
            expect(LPAREN);
            REG * expression2 = parse_expr(); //!< pointer to parsed second expression
            expression1->final->first_neighbor = expression2->start;//!< make expr1 final state point to expr2 start
            expression1->final->first_label = '_';//!< create eosilon transition between the two
            reg->start = expression1->start;//!< make the top level reg start = to expr1 start
            reg->final = expression2->final;//!< make the final state = to expr2 fial state
            //delete expression1;//!< free the intermediary REG
            expect(RPAREN);
        }
        else if(tt.token_type == OR) //!< union
        {
            expect(LPAREN);//!< consume the rparen so expr2 follows rule: expr -> CHAR
            REG * expression2 = parse_expr(); //!< pointer to parsed second expression
            reg->start = new State(); //!< inititialize start state
            reg->final = new State;//!< initailize final state
            reg->start->first_neighbor = expression1->start;
            reg->start->second_neighbor = expression2->start; 
            reg->start->first_label = '_';//!< epsilon transitions from start
            reg->start->second_label = '_';//!< epsilon transitions from start
            expression1->final->first_neighbor = reg->final;//!< linking expr1 final state
            expression2->final->first_neighbor = reg->final;//!< linking expr2 final state
            //delete expression1, expression2;//!< free the intermediary REG
            expect(RPAREN);
        }
        else if(tt.token_type == STAR) //!< kleene star
        {
            reg->start = new State();//!< initialize start state
            reg->final = new State();//!< inialize final state
            reg->start->first_neighbor = expression1->start;//!< linking expr1 start
            reg->start->first_label = '_';//!< add epsilon transition
            expression1->final->first_neighbor = reg->final;//!< linking final state to expr2
            expression1->final->first_label = '_';//!< add epsilon transition
            expression1->final->second_neighbor = expression1->start;//!< add the transition for kleene star
            expression1->final->second_label = '_';//!< make it an epsilon transition
            //delete expression1;//!< free the intermediary REG
        }
        else{syntax_error();}
    }
    else if(t.token_type == UNDERSCORE) //!< epsilon
    {
        reg->start = new State();//!< init start state
        reg->final = new State();//!< init final state
        reg->start->first_neighbor = reg->final;//!< add transition from start to final
        reg->start->first_label = '_';//!< make it an epsilon transition
    }
    else{syntax_error();}
    return reg;
}

int main()
{
    Parser parser;
    parser.parse_input();

}
