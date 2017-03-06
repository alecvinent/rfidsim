#ifndef RFIDSIMPP_RADIO_ANTENNA_H_
#define RFIDSIMPP_RADIO_ANTENNA_H_

#include <omnetpp.h>
#include <common/units.h>
#include <common/geometry.h>

namespace rfidsim {

class AntennaBase : public omnetpp::cSimpleModule {
public:
  virtual ~AntennaBase() {}

  virtual Decibel getLoss(double cos_x, double cos_y) const = 0;
  virtual Decibel getRxLoss(const Vector3& direction) const;
  virtual Decibel getTxLoss(const Vector3& direction) const;
};

class IsotropicAntenna : public AntennaBase {
public:
  virtual ~IsotropicAntenna() {}

  virtual Decibel getLoss(double cos_x, double cos_y) const {}
};

class DipoleAntenna : public AntennaBase {
public:
  virtual ~DipoleAntenna() {}

  virtual Decibel getLoss(double cos_x, double cos_y) const {}
};

}

#endif
