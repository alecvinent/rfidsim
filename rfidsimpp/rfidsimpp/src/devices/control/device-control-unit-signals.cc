#include <devices/control/device-control-unit-signals.h>

using namespace omnetpp;

namespace rfidsim {

Register_Class(DeviceCreated);
Register_Class(DeviceDestroyed);

const char *DeviceCreated::NAME = "DeviceCreated";
const char *DeviceDestroyed::NAME = "DeviceDestroyed";

std::string DeviceCreated::info() const
{
  static char buf[256];
  snprintf(buf, sizeof(buf), "%s {DeviceID=%d, device=%s <%p>}", NAME,
           device_id, (device ? device->getFullPath().c_str() : "none"),
           device);
  return buf;
}

std::string DeviceDestroyed::info() const
{
  static char buf[256];
  snprintf(buf, sizeof(buf), "%s {DeviceID=%d}", NAME, device_id);
  return buf;
}

}
