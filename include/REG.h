/*
 * Copyright (C) Caden Scharpf, 2021
 *
 * Do not share this file with anyone
 */

#include <vector>
#include <string>

#pragma once
/**
 * @class State
 * @brief Object to represent single a state in the REG NFA
 * 
 */
class State
{
    public:
        State();//!< constructor
        State * first_neighbor;//!< Next State upon reading a first_label
        State * second_neighbor;//!< Next State upon reading a second_label
        char first_label;//!< Input char to initiate transition to first_neighbor
        char second_label;//!< Input char to initiate transition to second_neighbor
        std::vector<State*> reachableBy(char p);
};

class REG
{
    public:
        REG();
        State * start;//< Start state
        State * final;//!< final state
        bool isfinal(State * state);
        std::vector <State *> epsilonClosure();
        bool match(std::string s);
};      