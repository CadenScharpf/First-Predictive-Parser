/*
 * Copyright (C) Caden Scharpf, 2021
 *
 * Do not share this file with anyone
 */

#include <vector>
#include <string>
#include "../include/unixcolors.h"
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
};//end class state

class REG
{
    public:
        REG();
        State * start;//!< Start state
        State * final;//!< final state
        bool isfinal(State * state);
        StateSet reachableBy(std::string s);//!<set of all possible states
                                            //  to be in after consuming s 
        StateSet epsilonClosure();
        bool match(std::string s);
        void print();

    private:
        StateSet reachableNodeAccumulator(std::string s);
        StateSet reachableByOne(StateSet * states, char input);
        StateSet epsilonAccumulator(StateSet * states);
        void printAccumulator(State * root, int depth, StateSet * ss);
}; //end class REG

// StateSet:: ------------------------------
class StateSetNode
{
    public:
    State * data;
    StateSetNode * next;
    StateSetNode(State * d, StateSetNode * n);
}; // end class StateSetNode

class StateSet
{
    public:
        StateSet();
        //~StateSet();
        int size;
        StateSetNode * head;
        bool push(State * s);
        bool equals(StateSet * s);
        bool contains(State * s);
        bool cat(StateSet * s);
        bool isEmpty();
        void print();
        void reset();
        State * getIdx(int idx);

    private:
        int garbageAccumulator(StateSetNode * head);
};// end class StateSet