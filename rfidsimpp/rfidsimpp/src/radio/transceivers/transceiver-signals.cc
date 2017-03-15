#include <radio/transceivers/transceiver-signals.h>

using namespace omnetpp;

namespace rfidsim {

Register_Class(TxStart);
Register_Class(TxFinish);

const char *TxStart::NAME = "TxStart";
const char *TxFinish::NAME = "TxFinish";

inline std::string _strTxSignal(const char *name, int device_id)
{
  char buf[256];
  snprintf(buf, sizeof(buf), "%s {DeviceID=%d}", name, device_id);
  return buf;
}

std::string TxStart::info() const
{
  return _strTxSignal(NAME, device_id);
}

std::string TxFinish::info() const
{
  return _strTxSignal(NAME, device_id);
}

}
