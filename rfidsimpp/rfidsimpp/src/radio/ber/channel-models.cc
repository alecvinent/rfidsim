#include <radio/ber/channel-models.h>

using namespace omnetpp;

namespace rfidsim {

Define_Module(ConstantChannel);

double ConstantChannel::getBER(double snr)
{
  return ber;
}

void ConstantChannel::initialize()
{
  ber = par("ber").doubleValue();
  if (ber < 0.0 || ber > 1.0)
    throw cRuntimeError("BER value=%.4f must be between 0.0 and 1.0", ber);
  if (ber > 0.5)
    EV_WARN << "BER should be in [0.0, 0.5], " << ber << " found" << std::endl;
}

}
