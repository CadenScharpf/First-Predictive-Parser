# Lexing with Non-Deterministic Automata

This simple program automatically implements a lexical analyzer for any list of tokens which are specified using regular expressions.  The input has two parts:

1. The first part of the input is a list of tokens separated by commas and terminated with the # (hash) symbol.  Each token in the list consists of a token name and a token description. The token description is a regular expression for the token. The list is of the following form:

t1_name t1_description , t2_name t2_description , ... , tk_name tk_description #

2. The second part of the input is an input string of letters/digits/spaces.

The program will read the list of tokens, represent them internally in appropriate data structures,and then do lexical analysis on the input string to break it down into a sequence of tokens and lexeme pairs from the provided list of tokens.  The output of the program is sequence of tokens and lexeme pairs.  If during the processing of the input string, the program cannot identify a token to match from the list, it will output ERROR and stops. If the input to the program has a syntax error, then the program will not perform any lexical analysis of the input string and instead will output a syntax error message and exits.
