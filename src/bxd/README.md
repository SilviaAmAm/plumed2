### How to use collective variables in a module

In the function “registerKeywords” add:

keys.use("ARG");

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

### How to obtain the positions in the BXD file

Then if one wanted to get the positions of all the atoms in the system, first one needs to create an array with the indices of all the atoms.

```c++
std::vector<AtomNumber> index(natoms);
for(int i = 0; i < natoms; i++)
{
    index[i].setIndex(i);
}
requestAtoms(index);
```

The function 'requestAtoms' doesn't return anything, I think it just makes the atoms requested available. The coordinates can then be obtained with the function 'getPosition' which returns a 'Vector', an alias for size 3 vectors in plumed. Here it is shown how to obtain the coordinates for atom 1.

```c++
Vector aPosition;   
aPosition = getPosition(1);
```

