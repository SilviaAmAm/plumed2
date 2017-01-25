### How to use collective variables in a bias module

In the function “registerKeywords” add:

keys.use("ARG");

This enables to use the keyword ‘ARG’ in the input file and thus to specify the label of a collective variable of interest. 
Then, in the “calculate” function one can use the function ‘getArgument’ which returns the value of the collective variable.