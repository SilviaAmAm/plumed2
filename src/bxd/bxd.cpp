#include "/Users/walfits/Repositories/plumed2/src/bias/Bias.h"
#include "/Users/walfits/Repositories/plumed2/src/bias/ActionRegister.h"

#include "/Users/walfits/Repositories/plumed2/src/core/ActionAtomistic.h"
#include "/Users/walfits/Repositories/plumed2/src/core/ActionRegister.h"
#include "/Users/walfits/Repositories/plumed2/src/core/PlumedMain.h"
#include "/Users/walfits/Repositories/plumed2/src/core/ActionSet.h"
#include "/Users/walfits/Repositories/plumed2/src/core/Atoms.h"

#include "/Users/walfits/Repositories/plumed2/src/tools/AtomNumber.h"


#include <string>
#include <cmath>
#include <cassert>
#include <iostream>
#include <vector>
#include <typeinfo>
#include <algorithm>

using namespace std;


namespace PLMD{
namespace bias{

class BXD :
    public Bias
{
    private:
      double boxSeparation;
      vector<double> boxes;
      bool isFirstStep;
    int currentBox(vector<double> &, const double);
      int theCurrentBox;
    
    public:
      explicit BXD(const ActionOptions&);
      static void registerKeywords(Keywords& keys);
      void calculate();
//    private:
//      vector<Atoms> atoms;
 
};
    
/* This function registers the main keyword for the new BXD class */
    
PLUMED_REGISTER_ACTION(BXD,"BXD")
    
    

/* This function is used to register any keywords that are needed for BXD to work */
    
void BXD::registerKeywords(Keywords& keys)
{
    Bias::registerKeywords(keys);
    keys.use("ARG");                // This enables to use collective variables
    keys.add("compulsory","BOXSEP","3.0","Defines the separation between the boxes");
}

    
/* This reads the values of the keywords that are specified in the input file (it is the constructor) */
    
BXD::BXD(const ActionOptions&ao):
    PLUMED_BIAS_INIT(ao),
    isFirstStep(true)
{
    parse("BOXSEP", boxSeparation);
    checkRead();
}

    
/* This is the function that does the BXD jobs */
    
void BXD::calculate()
{
    double ene;
    ene = getArgument(0);       //This is the energy of the system
    
    // Setting up the boxes
    
    boxes.resize(10);
    
    if(isFirstStep)
    {
        boxes[0] = 265;
        
        for(int i = 1; i < boxes.size(); i++)
        {
            boxes[i] = boxes[0] + i*boxSeparation;
        }
        
        isFirstStep = false;
    }
    
    theCurrentBox = currentBox(boxes, ene);
    cout << theCurrentBox << endl;
    
}
    

int BXD::currentBox(vector<double> &boxes, const double ene)
{
    int boxNumber = boxes.size();
    
    for(int i = 0; i < boxes.size(); i++)
    {
        if(ene < boxes[i])
        {
            return i;
        }
    }
    
    return boxNumber;
}
   
    
    
// Brackets from the namespaces!
}
}
