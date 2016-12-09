#include "/Users/walfits/Repositories/plumed2/src/bias/Bias.h"
#include "/Users/walfits/Repositories/plumed2/src/bias/ActionRegister.h"

#include <string>
#include <cmath>
#include <cassert>
#include <iostream>

using namespace std;


namespace PLMD{
namespace bias{

class BXD : public Bias
{
    public:
        explicit BXD(const ActionOptions&);
        static void registerKeywords(Keywords& keys);
        void calculate();
};
    
PLUMED_REGISTER_ACTION(BXD,"BXD")

void BXD::registerKeywords(Keywords& keys)
{
    Bias::registerKeywords(keys);
}

BXD::BXD(const ActionOptions&ao): PLUMED_BIAS_INIT(ao)
{
    checkRead();
}

void BXD::calculate()
{
    std::cout << "HELLOOOO!\n";
}
    
    
    

}
}
