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

### How to obtain the velocities from the BXD file




### How to change the positions from the BXD file

So far, to get the positions I have been using the public functions of ActionAtomistic that enable to work with the private instance of the Atom class within ActionAtomistic. However, ActionAtomistic has no method to modify the positions.

### How to modify the forces 

The current function of BXD manages to modify the forces

```c++
void BXD::apply()
{
    
    
    const unsigned numOfComp = getNumberOfComponents();
    const unsigned numOfArg = getNumberOfArguments();
    bool at_least_one_forced = false;
    std::vector<double> bxdForces(numOfArg,0.0);
    
    for(unsigned i = 0; i < numOfArg; ++i)
    {
        bxdForces[i] = 2;
    }
    
    
    std::vector<double> f(numOfArg, 0.0);
    std::vector<double> forces(numOfArg);
    
    if(onStep())
    {
        double gstr = static_cast<double>(getStride());
        for(unsigned i = 0; i < numOfArg; ++i)
        {
            getPntrToArgument(i)->addForce(gstr*bxdForces[i]);
        }
    }

    at_least_one_forced = true;
    
    for(unsigned i = 0; i < numOfComp; ++i)
    {
        getPntrToComponent(i)->applyForce(forces);
        for(unsigned j = 0; j < numOfArg; j++)
        {
            f[j] += forces[j];
            std::cout << forces[j] << "\t";
        }

    }
    
    if(at_least_one_forced)
    {
        for(unsigned i = 0; i < numOfArg; ++i)
        {
            getPntrToArgument(i)->addForce(f[i]);
        }
    }    
}
```
What needs to be understood:
 1. The bxdForces vector in this function is the equivalent of the bias vector 'outputForces'. Why does this vector has only a scalar value for the force on each collective variable rather than a vector? 
 
 The force seems to be the modulus of the force vector (from the restraint module). It is the force on the collective variable.
 
 2. How are these new bxdForces passed to the MD code?
 

 
In the bias class, they create an instance of the class 'Value'. This class enables you to keep together all the things related to a particular value, in this case the bias. The bias class sets the value of the energy as the 'value' and the force is the negative of the derivative of the 'value'. Every bias class has this. In 'ActionWithArguments' there is a public:

```c++
Value * getPntrToArgument
```

This points to the argument that was specified in the input file, so in the case of bias it is a collective variable. Then, in the Bias::apply() function, a force is added to the collective variable.

In PlumedMain.cpp there is a command "cmd_getBias" which converts the bias into MD units. It uses the function "getbias()" that is also in PlumedMain.cpp (bias is just a value in the class PlumedMain):

```c++
double PlumedMain::getBias() const{
  return bias;
}
```
Although, here they begin by defining bias as 0.0. I don't know where the actual bias value comes in.


The pointers to the MD objects are found in the class MDAtomsBase, located in the core/MDAtoms.h file. However, this is an abstract class and it is implemented in the MDAtomsTyped class (in core/MDAtoms.cpp).
An MDAtomsBase object is created inside the class Atoms, so one can access these pointers.

### How to change the velocities from the BXD file

