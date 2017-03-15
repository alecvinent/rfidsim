#ifndef RFIDSIMPP_RADIO_ANTENNAS_H_
#define RFIDSIMPP_RADIO_ANTENNAS_H_

#include <radio/antenna-base.h>
#include <common/rfidsim-defs.h>
#include <limits>

namespace rfidsim {

class IsotropicAntenna : public Antenna
{
 public:
  virtual ~IsotropicAntenna() {}

  virtual const char *getAntennaTypeName() const { return "Isotropic"; }

  virtual Decibel getLoss(double cos_fwd, double cos_right) const
  {
    return Decibel::ZERO;
  }
};

class DipoleAntenna : public Antenna
{
 public:
  static double COS_TOLERANCE;

  virtual ~DipoleAntenna() {}

  virtual const char *getAntennaTypeName() const { return "Dipole"; }

  virtual Decibel getLoss(double cos_fwd, double cos_right) const;

 protected:
  virtual void initialize(int stage);

 private:
  bool block_back_lobe;
};

}

#endif
