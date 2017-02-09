#include "/Users/walfits/Repositories/plumed2/src/core/ActionAtomistic.h"
#include "/Users/walfits/Repositories/plumed2/src/core/ActionWithArguments.h"
#include "/Users/walfits/Repositories/plumed2/src/core/ActionPilot.h"
#include "/Users/walfits/Repositories/plumed2/src/core/ActionRegister.h"
#include "/Users/walfits/Repositories/plumed2/src/core/PlumedMain.h"
#include "/Users/walfits/Repositories/plumed2/src/core/ActionWithValue.h"

#include "/Users/walfits/Repositories/plumed2/src/bias/Bias.h"

#include "/Users/walfits/Repositories/plumed2/src/tools/AtomNumber.h"


#include <string>
#include <cmath>
#include <cassert>
#include <iostream>
#include <vector>
#include <typeinfo>
#include <algorithm>

namespace PLMD{
namespace bias{

class BXD :
    public Bias,
    public ActionAtomistic
{
    private:
      double boxSeparation;
      int numBoxes;
      std::vector<double> boxes;
//      bool isFirstStep;
//      void setBoxes(const double);
    
      // Resolving the diamond inheritance from ActionAtomistic and ActionWithArguments
      bool lockRequestArguments;
      bool lockRequestAtoms;
      virtual void lockRequests();
      virtual void unlockRequests();
      virtual void calculateNumericalDerivatives( ActionWithValue* a=NULL ){};
    
    
    public:
      explicit BXD(const ActionOptions&);
      static void registerKeywords(Keywords& keys);
      virtual void calculate();  // pure virutal function that appears in Action.h and has to be defined in the most derived class
      virtual void apply(){};    // also a pure virtual function
 
};
    
/* This function registers the main keyword for the new BXD class */
    
PLUMED_REGISTER_ACTION(BXD,"BXD")
    
/* This function is used to register any keywords that are needed for BXD to work */
    
void BXD::registerKeywords(Keywords& keys)
{
    Bias::registerKeywords(keys);
    ActionAtomistic::registerKeywords(keys);
    keys.use("ARG");                // This enables to use collective variables
    keys.add("compulsory","BOXSEP","10.0","Defines the separation between the boxes");
    keys.add("compulsory","NUMBOXES","10","Defines the number of boxes to use");
}

    
/* This reads the values of the keywords that are specified in the input file (it is the constructor) */
    
BXD::BXD(const ActionOptions& ao):
    Action(ao),
    Bias(ao),
    ActionAtomistic(ao)
    //isFirstStep(true)
{
    parse("BOXSEP", boxSeparation);
    parse("NUMBOXES", numBoxes);
    boxes.resize(numBoxes);
    checkRead();
}

    
/* This is the function that calculates the values of the variables that will be changed by BXD */
    
void BXD::calculate()
{
    
    //This puts the value of the first collective variable specified in ARG in the variable colVar
    double colVar;
    colVar = getArgument(0);

    // This obtains the number of atoms in the system
    int natoms;
    natoms = plumed.getAtoms().getNatoms();

    // This creates a list of the indexes of the atoms of interests. In BXD that means all the atoms.
    std::vector<AtomNumber> index(natoms);
    
    for(int i = 0; i < natoms; i++)
    {
        index[i].setIndex(i);
    }
    requestAtoms(index);
    
    // This gets the position of seventh atom
    Vector aPosition;
    aPosition = getPosition(0);
    std::cout << aPosition[0] << "\t" << aPosition[1] << "\t" << aPosition[2] << std::endl;
    
    //This gets the current time step
    long int step;
    long int stepPlusOne;
    std::vector<double> masses;
    
    masses.resize(natoms);
    for(int i = 0; i < natoms; i++)
    {
        masses[i] = 0.0;
    }

    
    step = plumed.getStep();
    stepPlusOne = step + 1;
    
    //Trying to change the position
    std::vector<double> dd_coordinates;
    dd_coordinates.assign(3*natoms,0.0);
    
    plumed.cmd("setStep",&stepPlusOne);
    
    std::vector<Vector> pos(natoms);
    
    for(int i = 0; i < natoms; i++)
    {
        aPosition = getPosition(i);
        
        for(int j = 0; j < 3; j++)
        {
            pos[i][j] = aPosition[j];
        }
    }
    
    pos[0][1] = pos[0][1] + 3.0;
    pos[0][2] = pos[0][2] + 3.0;
    pos[0][3] = pos[0][3] + 3.0;
    
    //    std::cout << pos[0][1] << "\t" << pos[0][2] << "\t" << pos[0][3] << std::endl;
    
    plumed.cmd("setPositions",&pos[0][0]);
    
    std::vector<double> fake_forces( 3*natoms, 0.0 ), fake_virial(9);
    plumed.cmd("setForces",&fake_forces[0]);
    plumed.cmd("setVirial",&fake_virial[0]);
    
    aPosition = getPosition(0);
    
    std::cout << aPosition[0] << "\t" << aPosition[1] << "\t" << aPosition[2] << std::endl;
    std::cout << std::endl;
    
//    plumed.cmd("setMasses",&masses[0]);
//    plumed.cmd("setForces",&forces[0]);
//    plumed->cmd("setEnergy",&engconf);
//    plumed->cmd("setPositions",&positions[0]);
//    plumed->cmd("setBox",cell9);
//    plumed->cmd("setStopFlag",&plumedWantsToStop);
//    plumed->cmd("calc");
    
    
    
    
//    plumed.cmd("setStepLong",&step);
//    plumed.cmd("setPositions", &dd_coordinates[0]);

    

//    void * posPtr;
//    posPtr = &aPosition[0];
//    
//    plumed.getAtoms().setPositions(posPtr,1);
    
    
    
//    if(isFirstStep)
//    {
//        setBoxes(colVar);
//    }
    
}
    
/* This is the function that applies the changes calculated in the function 'calculate'. Both 'apply' and 'calculate' are virtual functions that are first introduced in Action.h and then have to be defined in the derived classes. */
    
//void apply()
//{
//    std::cout << "HELLO";
//}
    
    
/* This function uses the first value of the collective variable to set an array of barriers that define the boxes. */
    
//void BXD::setBoxes(const double firstColVar)
//{
//    boxes[0] = firstColVar - 0.5 * boxSeparation;
//    
//    for(int i = 1; i < numBoxes; i++)
//    {
//        boxes[i] = i*boxSeparation;
//    }
//}
    
    
/* Resolving the diamond inheritance problem. The virtual functions 'lockRequest' and 'unlockRequest' appear in the base class Action and in the two child classes ActionWithArguments and ActionAtomistic. BXD is the only class so far in PLUMED that inherits from both ActionWithArguments and ActionAtomistic, so a final overrider has to be present. For now, I just joined the two functions that were in ActionWithArguments and ActionAtomistic into one. */
    
void BXD::lockRequests()
{
    lockRequestArguments=true;
    lockRequestAtoms=true;
}

void BXD::unlockRequests()
{
    lockRequestArguments=false;
    lockRequestAtoms=false;
}
    
/* This is again a problem of diamond inheritance.  */
    
//void BXD::calculateNumericalDerivatives( ActionWithValue* a )
//{
//    
//}
//    
    
// Brackets from the namespaces!
}
}
