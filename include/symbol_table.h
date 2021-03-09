/*
 * Copyright (C) Caden Scharpf, 2021
 *
 */

#pragma once
#include <string>
#include "REG.h"
using namespace::std;

class Symbol
{
    public:
    Symbol(string newName, REG * newExpr);
    string name;
    REG * expr;
    Symbol * next;
};

class SymbolTable
{
    public:
    SymbolTable();
    Symbol * root;
    REG * lookup(string name);
    bool push(string name, REG * expr);

};