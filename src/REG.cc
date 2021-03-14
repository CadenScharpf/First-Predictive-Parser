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




//State:: -------------------------------------
State::State()
{
    first_neighbor = 0;
    second_neighbor = 0;
    first_label = 0;
    second_label = 0;
}

StateSet State::reachableBy(char p)
{
    StateSet reachable;
    if(first_label != 0 &&(first_label == p || first_label == '_')){reachable.push(first_neighbor);}
    if(second_label != 0 &&(second_label == p || second_label == '_')){reachable.push(second_neighbor);}
    return reachable;
}

void State::print()
{
    std::cout << "State {" << first_label << ", " << second_label << ", " 
              << first_neighbor << ", " << second_neighbor << "}" << std::endl;  
}

//StateSet::
StateSet::StateSet()
{
    size = 0;
    head = NULL;
}

StateSet::~StateSet()
{
    garbageAccumulator(head);
}

int StateSet::garbageAccumulator(state_set_t * head)
{
    if(head == NULL){return 0;}
    int count = garbageAccumulator(head->next);
    free(head);
    return count+1;
}

bool StateSet::push(State * s)
{
    state_set_t ** current = &head;
    while(*current)
    {
        if((s <= (*current)->data)){break;}
        current = & (*current)->next;
    }
    if(*current){if((*current)->data == s){return 1;}}//!< check if state* in the set
    state_set_t * newNode = (state_set_t *)malloc(sizeof(state_set_t));
    state_set_t * temp = *current;
    state_set_t buff = {s,temp};
    if(!newNode){return 0;}//!< mem check
    *newNode = buff;
    (*current) = newNode;
    return 1;
}

bool StateSet::equals(StateSet * s)
{
    if(s->size != size){return 0;}
    state_set_t ** current1 = &head;
    state_set_t ** current2 = &(s->head);
    while(*current1 && *current2)
    {
        if((*current1)->data != (*current2)->data){return 0;}
        current1 = & (*current1)->next;
        current2 = & (*current2)->next;
    }
    return 1;
}

bool StateSet::contains(State * s)
{
    state_set_t ** current = &head;
    while(*current)
    {
        if((*current)->data == s){return 1;}
        current = & (*current)->next;
    }
    return 0;
}

bool StateSet::cat(StateSet * s)
{
    state_set_t ** current = & (s->head);
    while (*current)
    {
        push((*current)->data);
        current = &(*current)->next;
    }
    return 1;
}

void StateSet::print()
{
    state_set_t ** curr = &head;
    while(*curr)
    {
        ((*curr)->data)->print();
        curr = &(*curr)->next;
    }
}

State * StateSet::getIdx(int idx)
{
    if(idx > size -1){return NULL;}
    StateSetNode ** current = &head;
    for(int i = idx; i > 0; i--){current = & (*current)->next;}
    return (*current)->data;
}

//REG:: ---------------------------------------
REG::REG()
{
    start = 0;
    final = 0;
}

StateSet REG::epsilonAccumulator(StateSet * states)
{
    StateSet reachable;
    reachable.cat(states);
     state_set_t ** curr = &(states->head);
     while(*curr)//!< add epsilon closure of states to reachable
     {
         StateSet adding = ((*curr)->data)->reachableBy('_');
         reachable.cat(&adding);
         curr = & (*curr)->next;
     }
     if(reachable.equals(states))
     {
        if(reachable.contains(final))
        {
             std::cout << "epsilon is nooooOOOoot a token\n";
             exit(1);
        }
        return reachable;
     }
     else
     {
        return epsilonAccumulator(&reachable);
     }
}// end method epsilon accumulator

StateSet REG::epsilonClosure()
{
    StateSet s = start->reachableBy('_');
    return epsilonAccumulator(&s);
}

bool REG::isfinal(State * state)
{
    if(final == state){return true;}
    return false;
}

StateSet REG::reachableBy(std::string s)
{
    return reachableNodeAccumulator(s);
}

StateSet REG::reachableByOne(StateSet * states, char input)
{
    StateSet reachable;
    state_set_t ** curr = &(states->head);
    while(*curr)
    {
        State * state = (*curr)->data;
        StateSet s = state->reachableBy(input);
        reachable.cat(&s);
    }
    return reachable;
}

StateSet REG::reachableNodeAccumulator(std::string s)
{
    if(s.length() == 0)//!< base case
    {
       return epsilonClosure();
    }
    else
    {
        StateSet r =  reachableNodeAccumulator(s.substr(1));
        return reachableByOne(&r, s[0]);
    }
}

bool REG::match(std::string s)
{
    StateSet reachable = reachableBy(s);
    if(reachable.contains(final))
    {
        return 1; // s is accepting
    }
    else
    {
        return 0; // s not accepting
    }
}// end method match
