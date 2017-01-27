
#include "/Users/walfits/Repositories/plumed2/src/core/ActionAtomistic.h"
#include "/Users/walfits/Repositories/plumed2/src/core/ActionWithArguments.h"
#include "/Users/walfits/Repositories/plumed2/src/core/ActionPilot.h"


#include "/Users/walfits/Repositories/plumed2/src/tools/AtomNumber.h"


#include <string>
#include <cmath>
#include <cassert>
#include <iostream>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <typeinfo>

using namespace std;


namespace PLMD{

class BXD :
    public ActionWithArguments,
    public ActionAtomistic,
    public ActionPilot
{
    private:
      double boxSeparation;
      int numBoxes;
      vector<double> boxes;
      bool isFirstStep;
      void setBoxes(const double);
      vector<AtomNumber> listAtoms;
    
    public:
      explicit BXD(const ActionOptions&);
      static void registerKeywords(Keywords& keys);
      void calculate();
      void apply();
 
};
    
/* This function registers the main keyword for the new BXD class */
    
PLUMED_REGISTER_ACTION(BXD,"BXD")
    
    

/* This function is used to register any keywords that are needed for BXD to work */
    
void BXD::registerKeywords(Keywords& keys)
{
    BXD::registerKeywords(keys);
    keys.use("ARG");                // This enables to use collective variables
    keys.add("atoms","GROUP","Hacky way to obtain the coordinates of the atoms in the system");
    keys.add("compulsory","BOXSEP","10.0","Defines the separation between the boxes");
    keys.add("compulsory","NUMBOXES","10","Defines the number of boxes to use");
}

    
/* This reads the values of the keywords that are specified in the input file (it is the constructor) */
    
BXD::BXD(const ActionOptions&ao):
    Action(ao),
    ActionAtomistic(ao),
    ActionPilot(ao),
    ActionWithArguments(ao),
    isFirstStep(true)
{
    parseAtomList("GROUP", listAtoms);
    parse("BOXSEP", boxSeparation);
    parse("NUMBOXES", numBoxes);
    boxes.resize(numBoxes);
    checkRead();
}

    
/* This is the function that does the BXD jobs */
    
void BXD::calculate()
{
    double colVar;
    colVar = getArgument(0);                    //This puts the value of the first collective variable specified in ARG in colVar
    
    setBoxes(colVar);
    
    vector<double> positions;
    positions.resize(3);
    positions = getPositions()
    
    cout << typeid(listAtoms[21]).name() << endl;
    


    
}
    
    
/* This function uses the first value of the collective variable to set an array of barriers that define the boxes. */
    
void BXD::setBoxes(const double firstColVar)
{
    boxes[0] = firstColVar - 0.5 * boxSeparation;
    
    for(int i = 1; i < numBoxes; i++)
    {
        boxes[i] = i*boxSeparation;
    }
    
}
   
    
    
// Brackets from the namespaces!
}
