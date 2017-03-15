#include <radio/medium/medium.h>
#include <radio/antenna-base.h>

namespace rfidsim {

Define_Module(WaveguideMedium);

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
