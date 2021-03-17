/*
 * Copyright (C) Caden Scharpf, 2021
 *
 * Do not share this file with anyone
 */
#include "REG.h"
#include <iostream>


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
    if(first_label != 0 &&(first_label == p )){reachable.push(first_neighbor);}
    if(second_label != 0 &&(second_label == p )){reachable.push(second_neighbor);}
    return reachable;
}

void State::print()
{
    std::cout << "State {" << first_label << ", " << second_label << ", " 
              << first_neighbor << ", " << second_neighbor << "}" << std::endl;  
}

//StateSet:: ---------------------------------------
StateSet::StateSet(){size = 0; head = NULL;};

//StateSet::~StateSet(){garbageAccumulator(head);};

StateSetNode::StateSetNode(State * d, StateSetNode * n){data = d; next = n;};

int StateSet::garbageAccumulator(StateSetNode * head)
{
    if(head == NULL){return 0;}
    int count = garbageAccumulator(head->next);
    delete head;
    return count+1;
}

bool StateSet::push(State * s)
{
    StateSetNode ** current = &head;
    while(*current)
    {
        if((s <= (*current)->data)){break;}
        current = & (*current)->next;
    }
    if(*current){if((*current)->data == s){return 1;}}//!< check if state* in the set
    StateSetNode * newNode = new StateSetNode(s,*current);
    if(!newNode){return 0;}//!< mem check
    (*current) = newNode;
    return 1;
}

bool StateSet::equals(StateSet * s)
{
    if(s->size != size){return 0;}
    StateSetNode ** current1 = &head;
    StateSetNode ** current2 = &(s->head);
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
    StateSetNode ** current = &head;
    while(*current)
    {
        if((*current)->data == s){return 1;}
        current = & (*current)->next;
    }
    return 0;
}

bool StateSet::cat(StateSet * s)
{
    StateSetNode ** current = & (s->head);
    while (*current)
    {
        push((*current)->data);
        current = &(*current)->next;
    }
    return 1;
}

void StateSet::print()
{
    StateSetNode ** curr = &head;
    while(*curr)
    {
        ((*curr)->data)->print();
        curr = &(*curr)->next;
    }
}

void StateSet::reset()
{
    garbageAccumulator(head);
    head = NULL;
    size = 0;
}

State * StateSet::getIdx(int idx)
{
    if(idx > size -1){return NULL;}
    StateSetNode ** current = &head;
    for(int i = idx; i > 0; i--){current = & (*current)->next;}
    return (*current)->data;
}

bool StateSet::isEmpty(){ if(size==0) {return 1;} else{return 0;}}

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
     StateSetNode ** curr = &(states->head);
     while(*curr)//!< add epsilon closure of states to reachable
     {
         StateSet adding = ((*curr)->data)->reachableBy('_');
         reachable.cat(&adding);
         curr = & (*curr)->next;
     }
     if(reachable.equals(states))
     {
        /*
        */
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
    s.push(start);
    StateSet buff = epsilonAccumulator(&s);
    /*
    if(buff.contains(final))
    {
        std::cout << "epsilon is nooooOOOoot a token\n";
        exit(1);
    }
    */
    return buff;
}

bool REG::isfinal(State * state)
{
    if(final == state){return true;}
    return false;
}

StateSet REG::reachableBy(std::string s)
{
    StateSet ss = reachableNodeAccumulator(s);
    ss.print();
    return ss;
}

StateSet REG::reachableByOne(StateSet * states, char input)
{
    // 1. Locate all nodes that can be reached directly from s by consuming c
    StateSet reachable;
    StateSetNode ** curr = &(states->head);
    while(*curr)//!< 
    {
        State * state = (*curr)->data;
        StateSet s = state->reachableBy(input);
        reachable.cat(&s);
        curr = & (*curr)->next;
    }
    if(reachable.isEmpty()) {return reachable;};

    // 2. Find all nodes that can be reached from the resulting set 'reachable by consuming no input
    return epsilonAccumulator(&reachable);

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


void REG::print(){StateSet * ss = new StateSet(); printAccumulator(start, 0, ss);}

void REG::printAccumulator(State * s, int depth, StateSet * ss)
{
    using namespace std;
    ss->push(s);

    string spacer = "";
    for(int i = depth;i>0;i--){spacer = spacer + " ";}
    string color = isfinal(s)? RED:YELLOW;
    cout << spacer << "State: " << color << s << RESET << endl;

    if(s->first_neighbor)
    {
        State * n1 = s->first_neighbor;
        string color = isfinal(n1)? RED:YELLOW;
        cout << spacer << "Left: " << color << n1 << RESET << " upon reading "  
             << YELLOW << s->first_label << RESET << endl;
        if(!ss->contains(n1))
        {
            printAccumulator(s->first_neighbor, depth+1, ss);
        };
    }
    if(s->second_neighbor)
    {
        State * n2 = s->second_neighbor;
        string color = isfinal(n2)? RED:YELLOW;
        cout << spacer << "Right: " << color << n2 << RESET << " upon reading " 
             << YELLOW << s->second_label << RESET << endl;
        if(!ss->contains(s->second_neighbor))
        {
          printAccumulator(s->second_neighbor, depth+1, ss);  
        }
    }
}