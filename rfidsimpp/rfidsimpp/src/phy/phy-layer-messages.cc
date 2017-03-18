#include <phy/phy-layer-messages.h>

using namespace omnetpp;

namespace rfidsim {

const char *ReplyTimeoutInd::NAME = "ReplyTimeoutInd";

const char *str(PhyResponseType type)
{
  switch (type) {
    case PHY_RESPONSE_OK: return "OK";
    case PHY_RESPONSE_COLLISION: return "Collision";
    case PHY_RESPONSE_CHANNEL_ERROR: return "Channel Error";
    default: throw cRuntimeError("unrecognized PhyResponseType = %d", type);
  }
}

Register_Class(ReplyTimeoutInd);

std::string ReplyTimeoutInd::info() const
{
  char buf[64];
  snprintf(buf, sizeof(buf), "%s {response=%s}", NAME,
           str(static_cast<PhyResponseType>(getResponse())));
  return buf;
}

}
