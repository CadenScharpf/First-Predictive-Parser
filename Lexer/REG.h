
class State
{
    public:
        State();
        State * first_neighbor;
        State * second_neighbor;
        char first_label;
        char second_label;
};

class REG
{
    public:
        State root;
};      