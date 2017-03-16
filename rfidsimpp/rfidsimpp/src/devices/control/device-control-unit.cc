#include <devices/control/device-control-unit.h>
#include <common/rfidsim-defs.h>
#include <common/module-access.h>
#include <world/life/life-units.h>

using namespace omnetpp;

namespace rfidsim {

Define_Module(DeviceControlUnit);

simsignal_t DeviceControlUnit::DEVICE_CREATED_SIGNAL_ID =
        registerSignal(DeviceCreated::NAME);

simsignal_t DeviceControlUnit::DEVICE_DESTROYED_SIGNAL_ID =
        registerSignal(DeviceDestroyed::NAME);

simsignal_t DeviceControlUnit::DESTROY_DEVICE_SIGNAL_ID =
        registerSignal(DestroyDevice::NAME);

DeviceControlUnit::~DeviceControlUnit()
{
  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->unsubscribe(i->first, this);
}

void DeviceControlUnit::destroy()
{
  DestroyDevice destroy_device_signal(device_id);
  emit(DESTROY_DEVICE_SIGNAL_ID, &destroy_device_signal);

  destroyed = true;

  DeviceDestroyed device_destroyed_signal(device_id);
  emit(DEVICE_DESTROYED_SIGNAL_ID, &device_destroyed_signal);
}

int DeviceControlUnit::numInitStages() const
{
  return INIT_STAGE_REGISTER_DEVICE + 1;
}

void DeviceControlUnit::initialize(int stage)
{
  auto device = getEnclosingRFIDDevice(this);
  auto thing = getEnclosingThing(this);
  if (stage == INIT_STAGE_BASE)
  {
    device_id = static_cast<int>(par("deviceID").longValue());
    destroyed = false;

    subscriptions.clear();
    subscriptions[LifeUnit::DESTROY_THING_SIGNAL_ID] = thing;

    for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
      i->second->subscribe(i->first, this);
  }
  else if (stage == INIT_STAGE_REGISTER_DEVICE)
  {
    DeviceCreated signal_(device_id, device);
    emit(DEVICE_CREATED_SIGNAL_ID, &signal_);
  }
}

void DeviceControlUnit::receiveSignal(
        cComponent *source, simsignal_t signal_id, cObject *obj,
        cObject *details)
{
  if (signal_id == LifeUnit::DESTROY_THING_SIGNAL_ID)
  {
    auto thing = getEnclosingThing(this);
    int thing_id = static_cast<int>(thing->par("thingID").longValue());
    auto destroy_thing = static_cast<DestroyThing*>(obj);
    if (thing_id == destroy_thing->thing_id)
      processDestoryThing(*destroy_thing);
  }
}

void DeviceControlUnit::processDestoryThing(const DestroyThing& upd)
{
  destroy();
}

}
