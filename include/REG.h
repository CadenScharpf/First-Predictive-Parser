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
        std::vector<State*> reachableBy(char p);//!< 
};

typedef struct StateSetNode
{
    State * data;
    State * next;
}state_set_t;

class StateSet
{
    public:
    StateSet();
    int size;
    StateSetNode * head;
    bool push(State * s);
    bool equals(StateSet * s);
    bool contains(State * s);
    bool cat(StateSet * s);
};

class REG
{
    public:
        REG();
        State * start;//!< Start state
        State * final;//!< final state
        bool isfinal(State * state);
        std::vector<State *> reachableBy(std::string s);//!<set of all possible states
                                                        //  to be in after consuming s 
        bool match(std::string s);

    private:
        std::vector <State *> epsilonClosure();
        std::vector <State *> reachableNodeAccumulator(std::string s);
        std::vector<State *> reachableByOne(std::vector<State *> states, char input);
        std::vector<State *> epsilonAccumulator(std::vector<State *> states);

};  

