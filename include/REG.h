/*
 * Copyright (C) Caden Scharpf, 2021
 *
 * Do not share this file with anyone
 */

#include <vector>
#include <string>
#pragma once

class StateSet;

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
        StateSet reachableBy(char p);//!< 
        void print();
};

class StateSetNode
{
    public:
    State * data;
    StateSetNode * next;
    StateSetNode(State * d, StateSetNode * n);
};

class StateSet
{
    public:
    StateSet();
    ~StateSet();
    int size;
    StateSetNode * head;
    bool push(State * s);
    bool equals(StateSet * s);
    bool contains(State * s);
    bool cat(StateSet * s);
    void print();
    State * getIdx(int idx);

    private:
        int garbageAccumulator(StateSetNode * head);
};

class REG
{
    public:
        REG();
        State * start;//!< Start state
        State * final;//!< final state
        bool isfinal(State * state);
        StateSet reachableBy(std::string s);//!<set of all possible states
                                                        //  to be in after consuming s 
        bool match(std::string s);

    private:
        StateSet epsilonClosure();
        StateSet reachableNodeAccumulator(std::string s);
        StateSet reachableByOne(StateSet * states, char input);
        StateSet epsilonAccumulator(StateSet * states);

};  

