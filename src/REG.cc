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

std::vector<State*> State::reachableBy(char p)
{
    std::vector<State*> reachable;
    if(first_label == p || first_label == '_'){reachable.push_back(first_neighbor);}
    if(second_label ==p || first_label == '_'){reachable.push_back(second_neighbor);}
    return reachable;

}


REG::REG()
{
    start = 0;
    final = 0;
}
std::vector <State *> REG::epsilonClosure()
{
    std::vector <State *> ret;
    if(start->first_label == '_'){ret.push_back(start->first_neighbor);}
    if(start->second_label == '_'){ret.push_back(start->second_neighbor);}
}

bool REG::match(std::string s)
{
    using namespace std;
    
}

bool REG::isfinal(State * state)
{
    if(final == state){return true;}
    return false;
}

bool REG::match(string s)