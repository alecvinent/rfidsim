#include <power/power-unit-signals.h>

using namespace omnetpp;

namespace rfidsim {

Register_Class(PowerOff);
Register_Class(PowerOn);
Register_Class(PowerUpdated);

const char *PowerOn::NAME = "PowerOn";
const char *PowerOff::NAME = "PowerOff";
const char *PowerUpdated::NAME = "PowerUpdated";

std::string PowerOff::info() const
{
  static char buf[128];
  snprintf(buf, sizeof(buf), "%s {DeviceID=%d}", NAME, device_id);
  return buf;
}

std::string PowerOn::info() const
{
  static char buf[256];
  snprintf(buf, sizeof(buf), "%s {DeviceID=%d, power=%s}", NAME, device_id,
           power.info().c_str());
  return buf;
}

std::string PowerUpdated::info() const
{
  static char buf[256];
  snprintf(buf, sizeof(buf), "%s {DeviceID=%d, power=%s}", NAME, device_id,
           power.info().c_str());
  return buf;
}

}
