#ifndef RFIDSIMPP_RADIO_MEDIUM_MEDIUM_H_
#define RFIDSIMPP_RADIO_MEDIUM_MEDIUM_H_

#include <omnetpp.h>
#include <common/units.h>

namespace rfidsim {

class Antenna;

class Medium : public omnetpp::cSimpleModule {
 public:
  virtual ~Medium() {}

  virtual Decibel getPathLoss(
          Antenna *tx_antenna, Antenna *rx_antenna) const = 0;

  virtual Power getThermalNoise() = 0;
};


class WaveguideMedium : public Medium {
 public:
  virtual ~WaveguideMedium() {}

  virtual Decibel getPathLoss(
          Antenna *tx_antenna, Antenna *rx_antenna) const;

  virtual Power getThermalNoise();

 protected:
  virtual void initialize();

 private:
  Decibel loss = Decibel::ZERO;
  Power noise = Power::ZERO;
};

}

#endif
