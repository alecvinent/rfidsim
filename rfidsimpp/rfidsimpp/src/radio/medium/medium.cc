#include <radio/medium/medium.h>
#include <radio/antenna-base.h>

using namespace omnetpp;

namespace rfidsim {

Define_Module(WaveguideMedium);

const double Medium::SPEED_OF_LIGHT = 299792458.0;

simtime_t Medium::getPropagationDelay(
        Antenna *tx_antenna, Antenna *rx_antenna) const
{
  auto p_tx = tx_antenna->getAbsolutePosition();
  auto p_rx = rx_antenna->getAbsolutePosition();
  auto d = p_tx.getDistanceTo(p_rx);
  return d / SPEED_OF_LIGHT;
}

//
//===========================================================================
// WaveguideMedium
//===========================================================================
//
Decibel WaveguideMedium::getPathLoss(
        Antenna *tx_antenna, Antenna *rx_antenna) const
{
  return loss;
}

Power WaveguideMedium::getThermalNoise()
{
  return noise;
}

void WaveguideMedium::initialize()
{
  loss = Decibel(par("loss").doubleValue());
  noise = Power(par("noise").doubleValue(), Power::DBM);
}

}
