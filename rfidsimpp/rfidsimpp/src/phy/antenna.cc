#include <phy/antenna.h>

namespace rfidsim {

Define_Module(IsotropicAntenna);
Define_Module(DipoleAntenna);

Decibel AntennaBase::getRxLoss(const Vector3& direction) const {
  //TODO
  return Decibel(0.0);
}

Decibel AntennaBase::getTxLoss(const Vector3& direction) const {
  //TODO
  return Decibel(0.0);
}

}
