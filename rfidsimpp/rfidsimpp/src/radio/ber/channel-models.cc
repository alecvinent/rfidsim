#include <radio/ber/channel-models.h>

using namespace omnetpp;

namespace rfidsim {

Define_Module(ConstantBERChannelModel);
Define_Module(AWGNChannelModel);
Define_Module(RayleighChannelModel);

void ConstantBERChannelModel::initialize()
{
  ber = par("ber").doubleValue();
  if (ber < 0.0 || ber > 1.0)
    throw cRuntimeError("BER value=%.4f must be between 0.0 and 1.0", ber);
  if (ber > 0.5)
    EV_WARN << "BER should be in [0.0, 0.5], " << ber << " found" << std::endl;
}


double AWGNChannelModel::getBER(double snr)
{
  double t = 0.5 - 0.5 * erf(sqrt(snr/2));
  return 2 * t * (1 - t);
}

double RayleighChannelModel::getBER(double snr)
{
  double t = sqrt(1 + 2 / snr);
  return 0.5 + (2 * atan(t) - M_PI) / (M_PI * t);
}

}
