### How to use collective variables in BXD

In the function “registerKeywords” add:

```c++
Bias::registerKeywords(keys);
keys.use("ARG");
```
One has to use the registerKeywords functions from all the classes you inherit from. 

This enables to use the keyword ‘ARG’ in the input file and thus to specify the label of a collective variable of interest. 
Then, in the “calculate” function one can use the function ‘getArgument’ which returns the value of the collective variable.

In order to be able to do this you need to include the ActionRegister.h file. 




### How to obtain the number of atoms in the system from the BXD file:

One needs to include the header core/PlumedMain.h.
Then, one can use the main plumed object and do this:

```c++
int natoms;
natoms = plumed.getAtoms().getNatoms();
```
One is able to call the main PlumedMain object 'plumed' because it is a public member in the class Action.

Another way of doing it is to use the fact that the class ActionAtomistic has an object Atom and has a member function that enables to acces the total number of atoms.

```c++
int natoms;
natoms = getTotAtoms();
```

### How to obtain the positions in the BXD file

Then if one wanted to get the positions of all the atoms in the system, first one needs to create an array with the indices of all the atoms. These are functions that can be used because they are inherited from ActionAtomistic.

```c++
std::vector<AtomNumber> index(natoms);
for(int i = 0; i < natoms; i++)
{
    index[i].setIndex(i);
}
requestAtoms(index);
```

The function 'requestAtoms' doesn't return anything, I think it just makes the atoms requested available. The coordinates can then be obtained with the function 'getPosition' which returns a 'const Vector' or with 'modifyPositions' which returns a 'Vector'. Vector is an alias for size 3 vectors in plumed. Here it is shown how to obtain the coordinates for atom 1.

```c++
Vector aPosition;   
aPosition = modifyPosition(index[0]);
```

### How to obtain the step number

One can obtain the number of time steps performed as follows:

```c++
long int step;
step = plumed.getStep();
```
## Common problems

### Complains about labels not recognised

```
message: ERROR in input to action BXD with label @1 : cannot understand the following words from the input line : LABEL=bxd, 
Abort trap: 6
```
Make sure that you have registered the keywords from Action:

```
void BXD::registerKeywords(Keywords& keys)
{
    Action::registerKeywords( keys );       // <----- This!
    ActionAtomistic::registerKeywords(keys);
}
```

### The function 'calculate' isn't called

All the actions in plumed are run only when another action needs its output as an input. For example, if you have in the plumed input file:
```
d: DISTANCE ATOMS=1,2
```
Then, the distance is not actually calculated. However, if you have:
```
d: DISTANCE ATOMS=1,2
PRINT ARG=d FILE=COLVAR
```
Then the distance is calculated because the action PRINT needs it as an input.

If you want to create an action whose output doesnt need to be input into something else, you need to make this new action inherit from the ActionPilot class. This class drives the execution of other Action's at a fixed stride which is specified with the command STRIDE= keyword.

