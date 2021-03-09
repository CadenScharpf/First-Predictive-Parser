/*
 * Copyright (C) Caden Scharpf, 2021
 *
 * Do not share this file with anyone
 */
#include "../include/REG.h"

State::State()
{
    first_neighbor = 0;
    second_neighbor = 0;
    first_label = 0;
    second_label = 0;
}

REG::REG()
{
    start = new State();
    final = 0;
}

bool REG::isfinal(State * state)
{
    if(final == state){return true;}
    return false;
}