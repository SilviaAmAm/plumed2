#include "/Users/walfits/Repositories/plumed2/src/bias/Bias.h"
#include "/Users/walfits/Repositories/plumed2/src/bias/ActionRegister.h"
#include "/Users/walfits/Repositories/plumed2/src/core/ActionAtomistic.h"

#include <string>
#include <cmath>
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;


namespace PLMD{
namespace bias{

class BXD : public Bias
{
    public:
      explicit BXD(const ActionOptions&);
      static void registerKeywords(Keywords& keys);
      void calculate();
    
    private:
      vector<double> box_sep;
      vector<double> atom_list;
};
    
/* This function registers the main keyword for the new BXD class */
    
PLUMED_REGISTER_ACTION(BXD,"BXD")
    
    

/* This function is used to register any keywords that are needed for BXD to work */
    
void BXD::registerKeywords(Keywords& keys)
{
    Bias::registerKeywords(keys);
}

    
/* This reads the values of the keywords that are specified in the input file */
    
BXD::BXD(const ActionOptions&ao): PLUMED_BIAS_INIT(ao)
{
    checkRead();
}

    
/* This is the function that does the BXD jobs */
    
void BXD::calculate()
{
    // I want to find a way to obtain all the positions of the atoms
    
}
    
    
    

}
}
