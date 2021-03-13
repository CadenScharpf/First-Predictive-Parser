/*
 * Copyright (C) Caden Scharpf, 2021
 *
 * Do not share this file with anyone
 */
#include "../include/REG.h"
#include <iostream>

void vectorCat(std::vector<State *>& v1, std::vector<State *> v2)
{
    v1.insert(v1.end(), v2.begin(), v2.end());
}

State::State()
{
    first_neighbor = 0;
    second_neighbor = 0;
    first_label = 0;
    second_label = 0;
}
StateSet::StateSet()
{
    size = 0;
}
REG::REG()
{
    start = 0;
    final = 0;
}
std::vector<State*> State::reachableBy(char p)
{
    std::vector<State*> reachable;
    if(first_label != 0 &&(first_label == p || first_label == '_')){reachable.push_back(first_neighbor);}
    if(second_label != 0 &&(second_label == p || second_label == '_')){reachable.push_back(second_neighbor);}
    return reachable;
}



std::vector<State *> REG::epsilonAccumulator(std::vector<State *> states)
{
    std::vector<State*> reachable;
    vectorCat(reachable, states);
     for(int i = 0; i < states.size(); i++)
     {
         vectorCat(reachable, states[i]->reachableBy('_'));
     }
     if(reachable == states)
     {
        if(std::find(reachable.begin(), reachable.end(), final) != reachable.end())
        {
             std::cout << "epsilon is nooooOOOoot a token\n";
             exit(1);
        }
        return reachable;
     }
     else
     {
        return epsilonAccumulator(reachable);
     }
}// end method epsilon accumulator

std::vector <State *> REG::epsilonClosure()
{
    return epsilonAccumulator(start->reachableBy('_'));
}

bool REG::isfinal(State * state)
{
    if(final == state){return true;}
    return false;
}

std::vector<State *> REG::reachableBy(std::string s)
{
    return reachableNodeAccumulator(s);
}
std::vector<State *> REG::reachableByOne(std::vector<State *> states, char input)
{
    std::vector<State *> reachable;
    for(int i = 0; i < states.size(); i++)
    {
        State * state = states[i];
        vectorCat(reachable,state->reachableBy(input));
    }
    return reachable;
}

std::vector<State *> REG::reachableNodeAccumulator(std::string s)
{
    if(s.length() == 0)//!< base case
    {
       return epsilonClosure();
    }
    else
    {
        return reachableByOne(reachableNodeAccumulator(s.substr(1)), s[0]);
    }
}

bool REG::match(std::string s)
{
    std::vector<State *> reachable = reachableBy(s);
    if(std::find(reachable.begin(), reachable.end(), &(*final)) != reachable.end())
    {
        return 1; // s is accepting
    }
    else
    {
        return 0; // s not accepting
    }
}// end method match
