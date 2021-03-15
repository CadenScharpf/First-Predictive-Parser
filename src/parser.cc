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

using namespace std;

SymbolTable token_table;

// Helpers ------------

void printVectorStrings(vector<string> v, string name)
{
    std::cout << (name + ": ") << std::endl;
    for(int i = 0; i < v.size(); i++)
    {
        std::cout << v[i] << std::endl;
    }
    std::cout << std::endl;
}

// --------------------

vector<string> tokenize(string s)
{
    vector<string> input_tokens;
    stringstream ss(s);
    string buff;
    ss>>buff;//!< skip the space
    while(ss >> buff){input_tokens.push_back(buff);}
    input_tokens.pop_back();
    return input_tokens;
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
/**
 * @brief Parse program input
 * 
 */
void Parser::parse_input() 
{
    parse_tokens_section();//!< Generate REG graphs
    // Tokenizing the input text
    vector<string> input_tokens = tokenize(expect(INPUT_TEXT).lexeme);
    printVectorStrings(input_tokens, "input_tokens");
    token_table.listSymbols();
    vector<string> tok_types; 
    

    for(int i = 0 ;i < input_tokens.size(); i++)//!< loop over input strings
    {   
        bool matched = 0;
        string token = input_tokens[i];
        for(int j = 0;j < token_table.size; j++)//!< loop over REG graphs(symbols)
        {
            Symbol * symbol = token_table.getIndex(j);
            REG * reg = symbol->expr;
            if(reg->match(input_tokens[j]))
            {
                matched = 1;
                tok_types.push_back(symbol->name);
                break;
            }
        }
        if(matched == 0){cout << "error: no match" << endl;exit(0);}
    }
    for(int i = 0; i<input_tokens.size(); i++)
    {
        cout << input_tokens[i] << ":" << tok_types[i]; 
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

    Token t = lexer.GetToken();//!< consume first character
    if(t.token_type == CHAR)
    {
        reg->start = new State();//!< initialize start state
        reg->final = new State();//!< initilize final state
        reg->start->first_neighbor = reg->final;//!< set transition state
        reg->start->first_label = t.lexeme[0];//!< set the transion label
    }
    else if(t.token_type == LPAREN)
    {
        REG * expression1 = parse_expr(); //!< pointer to parsed first expression
        expect(RPAREN);//!< consume/check for right paren

        Token tt = lexer.GetToken(); //!< consume manditory operator
        if(tt.token_type == DOT) //!< concatanation
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
            reg->start->second_label = '_';//!< epsilon transritions from start
            expression1->final->first_neighbor = reg->final;//!< linking expr1 final state
            expression2->final->first_neighbor = reg->final;//!< linking expr2 final state
            expression1->final->first_label = '_';
            expression2->final->first_label = '_';

            delete expression1;
            delete expression2;//!< free the intermediary REG
            expect(RPAREN);
        }
        else if(tt.token_type == STAR) //!< kleene star
        {
            reg->start = new State();//!< initialize start state
            reg->final = new State();//!< inialize final state
            reg->start->first_neighbor = expression1->start;//!< linking transition to f*
            reg->start->second_neighbor = reg->final;//!< linking transition to f* final
            reg->start->first_label = '_';//!< add epsilon transition to f*
            reg->start->second_label = '_';//add transition to accept epsilion
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
    //parser.parse_input();
    REG * r = parser.parse_expr();
    std::cout << r->match("ff") << std::endl;

}