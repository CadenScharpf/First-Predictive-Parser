#include "../include/REG.h"
#include ""
#include <iostream>
#include "../include/unixcolors.h"

using namespace std;

// Unit Tests -----------------------------
void testREG();

// Test Runner ----------------------------
void run_tests()
{
    testREG();
}

// Entry point ----------------------------
int main()
{
    cout << endl << BOLDCYAN << "Project 2: First Predictive Parser" << RESET << endl;
    cout <<  "Performing Unit Tests..." << endl << endl;
    run_tests();
}

// Test definitions -----------------------
void testREG()
{
    cout << YELLOW << "Testing REG structure..." << RESET << endl;
    
    string input_strings[] = 
    {
        "a",
        "",
        "",
        "",
        "",
    };
}
// End of file ---------------------------