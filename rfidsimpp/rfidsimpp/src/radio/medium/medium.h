#ifndef RFIDSIMPP_RADIO_MEDIUM_MEDIUM_H_
#define RFIDSIMPP_RADIO_MEDIUM_MEDIUM_H_

#include <omnetpp.h>
#include <common/units.h>

namespace rfidsim {

class Antenna;

class Medium : public omnetpp::cSimpleModule {
 public:
  static const double SPEED_OF_LIGHT;

  virtual ~Medium() {}

  virtual Decibel getPathLoss(
          Antenna *tx_antenna, Antenna *rx_antenna) const = 0;

  virtual Power getThermalNoise() = 0;

  omnetpp::simtime_t getPropagationDelay(
          Antenna *tx_antenna, Antenna *rx_antenns) const;
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
