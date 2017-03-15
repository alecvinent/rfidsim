#include <devices/control/device-control-units.h>
#include <devices/control/device-control-unit-signals.h>
#include <common/rfidsim-defs.h>
#include <common/module-access.h>

using namespace omnetpp;

namespace rfidsim {

simsignal_t DeviceControlUnit::DEVICE_CREATED_SIGNAL_ID =
        registerSignal(DeviceCreated::NAME);

simsignal_t DeviceControlUnit::DEVICE_DESTROYED_SIGNAL_ID =
        registerSignal(DeviceDestroyed::NAME);

void DeviceControlUnit::destroy()
{
  DeviceDestroyed signal_(device_id);
  emit(DEVICE_DESTROYED_SIGNAL_ID, &signal_);
}

int DeviceControlUnit::numInitStages() const
{
  return INIT_STAGE_REGISTER_DEVICE + 1;
}

void DeviceControlUnit::initialize(int stage)
{
  if (stage == INIT_STAGE_BASE)
  {
    device_id = static_cast<int>(par("deviceID").longValue());
  }
  else if (stage == INIT_STAGE_REGISTER_DEVICE)
  {
    auto device = getEnclosingRFIDDevice(this);
    DeviceCreated signal_(device_id, device);
    emit(DEVICE_CREATED_SIGNAL_ID, &signal_);
  }
}

void DeviceControlUnit::receiveSignal(
        cComponent *source, simsignal_t signal_id, cObject *obj,
        cObject *details)
{
}

}
