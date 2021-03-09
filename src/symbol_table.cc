/*
 * Copyright (C) Caden Scharpf, 2021
 *
 */

#include <iostream>
#include "../include/symbol_table.h"


Symbol::Symbol(string newName, REG * newExpr)
{
    name = newName;
    expr = newExpr;
    next = 0;
}
SymbolTable::SymbolTable()
{
    root = 0;
}

/**
 * @brief 
 * 
 * @param name symbol ID
 * @return REG* if the name exists in the table.
 *         Otherwise will return 0.
 */
REG * SymbolTable::lookup(string name)
{
    Symbol * skipper = root;
    while(skipper->next)
    {
        if(skipper->name == name)
        {
            return skipper->expr;
        }
    }
    return 0;
}

/**
 * @brief Push a new symbol onto the table
 * 
 * @param name ID of the new symbol
 * @param expr pointer to a REG
 * @return true 
 * @return false 
 */
bool SymbolTable::push(string name, REG * expr)
{
    Symbol * newSymbol = new Symbol(name, expr);
    if(newSymbol = 0)
    {
        cout << "ERROR: Failed to alocate space for a new symbol" << endl;
        return 0;
    }

    Symbol * skipper = root;
    while(skipper->next){skipper = skipper->next;};
    skipper->next = newSymbol;
    return 1;
}