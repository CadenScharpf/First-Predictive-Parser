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
    next = NULL;
}
SymbolTable::SymbolTable()
{
    root = NULL;
    size = 0;
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
    Symbol ** current = &root;
    while(*(current))
    {
        if( ((*current)->name) == name)
        {
            return ((*current)->expr);
        }
        current = & (*current)->next;
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
    Symbol * newSymbol = new (nothrow) Symbol(name, expr);
    if(newSymbol == 0)
    {
        cout << "ERROR: Failed to alocate space for a new symbol" << endl;
        return 0;
    }
 
    Symbol ** current = &root;
    while((*current)){current = & (*current)->next;};
    *current = newSymbol;
    size++;
    return 1;
}

/**
 * @brief Provides arraylike access to the list of Symbols
 * 
 * @param idx 'index' to be accessed
 * @return REG* NULL if idx < # symbols
 *              otherwise,
 *              REG* of the idx'th symbol in the list
 */
Symbol * SymbolTable::getIndex(int idx)
{
    if(idx > size-1){return NULL;}
    int pos = 0;
    Symbol ** current = &root;
    while(pos < idx){current = &(*current)->next; pos++;};
    return *current;
}