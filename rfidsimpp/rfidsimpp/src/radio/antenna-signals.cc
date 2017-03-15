#include <antenna-base.h>
#include <radio/antenna-signals.h>

using namespace omnetpp;

namespace rfidsim {

Register_Class(ReceivedFieldOff);
Register_Class(ReceivedFieldOn);
Register_Class(ReceivedFieldUpdated);

Register_Class(RadiatedFieldOff);
Register_Class(RadiatedFieldOn);
Register_Class(RadiatedFieldUpdated);

Register_Class(SwitchAntenna);
Register_Class(AntennaActivated);


const char *ReceivedFieldOff::NAME = "ReceivedFieldOff";
const char *ReceivedFieldOn::NAME = "ReceivedFieldOn";
const char *ReceivedFieldUpdated::NAME = "ReceivedFieldUpdated";

const char *RadiatedFieldOff::NAME = "RadiatedFieldOff";
const char *RadiatedFieldOn::NAME = "RadiatedFieldOn";
const char *RadiatedFieldUpdated::NAME = "RadiatedFieldUpdated";

const char *SwitchAntenna::NAME = "SwitchAntenna";
const char *AntennaActivated::NAME = "AntennaActivated";

std::string ReceivedFieldOff::info() const
{
  static char buf[128];
  snprintf(buf, sizeof(buf), "%s {ReceiverDeviceID=%d, PeerDeviceID=%d}", NAME,
           receiver_device_id, peer_device_id);
  return buf;
}

std::string ReceivedFieldOn::info() const
{
  static char buf[128];
  snprintf(buf, sizeof(buf), "%s {DeviceID=%d, PeerAntenna=%s, Power=%s}",
           NAME, device_id,
           (peer_antenna ? peer_antenna->getAntennaName().c_str() : "null"),
           power.info().c_str());
  return buf;
}

std::string ReceivedFieldUpdated::info() const
{
  static char buf[128];
  snprintf(buf, sizeof(buf), "%s {DeviceID=%d, PeerAntenna=%s Power=%s}", NAME,
           device_id,
           (peer_antenna ? peer_antenna->getAntennaName().c_str() : "null"),
           power.info().c_str());
  return buf;
}

std::string RadiatedFieldOff::info() const
{
  static char buf[128];
  snprintf(buf, sizeof(buf), "%s {DeviceID=%d}", NAME, device_id);
  return buf;
}

std::string RadiatedFieldOn::info() const
{
  static char buf[256];
  snprintf(buf, sizeof(buf), "%s {DeviceID=%d, PeerAntenna=%s, Power=%s}",
           NAME, device_id,
           (peer_antenna ? peer_antenna->info().c_str() : "null"),
           radiated_power.info().c_str());
  return buf;
}

std::string RadiatedFieldUpdated::info() const
{
  static char buf[256];
  snprintf(buf, sizeof(buf), "%s {DeviceID=%d, PeerAntenna=%s, Power=%s}",
           NAME, device_id,
           (peer_antenna ? peer_antenna->getAntennaName().c_str() : "null"),
           radiated_power.info().c_str());
  return buf;
}

std::string SwitchAntenna::info() const
{
  static char buf[256];
  snprintf(buf, sizeof(buf), "%s {DeviceID=%d, index=%d}", NAME, device_id,
           index);
  return buf;
}

std::string AntennaActivated::info() const
{
  static char buf[256];
  snprintf(buf, sizeof(buf), "%s {DeviceID=%d, index=%d, Antenna=%s}", NAME,
           device_id, index,
           (antenna ? antenna->getAntennaName().c_str() : "null"));
  return buf;
}

}
