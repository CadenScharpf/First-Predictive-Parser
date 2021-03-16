#pragma once
#include "REG.h"
#include "symbol_table.h"
#include <string>

using namespace std;

StateSet matchOneChar(StateSet S, char c);

typedef struct MyToken
{
    string token_type;
    string lexeme;
}token_t;

typedef struct Match
{
    token_t token;
    int priority;
    int len;
}match_t;

class my_LexicalAnalyzer
{
    public:
    my_LexicalAnalyzer(SymbolTable tokens, string s);
    token_t my_GetToken();
    char peek(int idx);
    char get();
    int pos;// current index to process from
    private:
    string inputText;//origional input str;
    SymbolTable tokentable;//!< list of {token_type, REG*} pairs
    string match(REG * r, string s, int p);
};

char my_LexicalAnalyzer::peek(int idx){return inputText[idx];}

my_LexicalAnalyzer::my_LexicalAnalyzer(SymbolTable tokens, string s)
{
    pos = 1;
    tokentable = tokens;
    inputText = s;
}

string my_LexicalAnalyzer::match(REG * r, string s, int p)
{
    int position = p;
    string lmp = "";
    while(inputText[p] == ' '){p++;}

    StateSet set = matchOneChar(r->epsilonClosure(),inputText[position]);
    if(!set.isEmpty()){return "";}
    position++;
    while(!set.isEmpty()&& position < inputText.length())
    {
        lmp = lmp + inputText[inputText[p]];
        StateSet temp = matchOneChar(r->epsilonClosure(), inputText[position]);
        set = temp;
        position++;
    }
    return lmp;
}

token_t my_LexicalAnalyzer::my_GetToken()
{
    cout << match(tokentable.lookup("t4"), inputText, 1) << endl;

}

StateSet matchOneChar(StateSet S, char c)
{
    StateSet Sprime;
    StateSetNode ** curr = &(S.head);
    while(*curr)//!< 
    {
        State * state = (*curr)->data;
        Sprime.push(state);
        StateSet s = state->reachableBy(c);
        Sprime.cat(&s);
        curr = & (*curr)->next;
    }
    if(Sprime.isEmpty()) {return Sprime;};

    // 2. Find all nodes that can be reached from the resulting set 'reachable by consuming no input
    bool changed = true;
    StateSet sdp;
    while(changed)
    {
        changed = false;
        StateSetNode ** current = &(Sprime.head);
        while(*current)
        {
            State * b = (*current)->data;
            sdp.push(b);
            StateSet buffer = b->reachableBy('_');
            sdp.cat(&buffer);
            current = & (*current)->next;
        }
        if(!Sprime.equals(&sdp))
        {
            changed = true;
            Sprime.reset();
            Sprime.cat(&sdp);
            sdp.reset();

        }
    }
    return Sprime;
}