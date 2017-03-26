#include <phy/phy-layer-messages.h>

using namespace omnetpp;

namespace rfidsim {

const char *PhyDataConf::NAME = "PhyDataConf";

const char *str(PhyDataConfStatus status)
{
  switch (status) {
    case PHY_DATA_CONF_OK: return "OK";
    case PHY_DATA_CONF_COLLISION: return "Collision";
    case PHY_DATA_CONF_CHANNEL_ERROR: return "Channel Error";
    case PHY_DATA_CONF_NO_REPLY: return "No Reply";
    default: throw cRuntimeError("unrecognized PhyDataConfStatus = %d", status);
  }
}

Register_Class(PhyDataConf);

std::string PhyDataConf::info() const
{
  char buf[64];
  snprintf(buf, sizeof(buf), "%s {status=%s}", NAME,
           str(static_cast<PhyDataConfStatus>(getStatus())));
  return buf;
}

}
