#include <radio/antenna-models.h>

using namespace omnetpp;

namespace rfidsim {

Define_Module(IsotropicAntenna);
Define_Module(DipoleAntenna);

//
//===========================================================================
// DipoleAntenna
//===========================================================================
//

double DipoleAntenna::COS_TOLERANCE = 1e-6;

Decibel DipoleAntenna::getLoss(double cos_fwd, double cos_right) const
{
  if (std::abs(cos_fwd) < COS_TOLERANCE || (block_back_lobe && cos_fwd < 0))
  {
    return Decibel(-std::numeric_limits<double>::infinity());
  }
  return std::abs(cos(M_PI_2 * cos_fwd) / cos_fwd);
}

void DipoleAntenna::initialize(int stage)
{
  Antenna::initialize(stage);
  if (stage == INIT_STAGE_BASE)
  {
    block_back_lobe = par("blockBackLobe").boolValue();
  }
}

}
