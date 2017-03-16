#include <power/power-unit-base.h>
#include <power/power-unit-signals.h>
#include <devices/control/device-control-unit.h>
#include <common/module-access.h>

using namespace omnetpp;

namespace rfidsim {

simsignal_t PowerUnit::POWER_ON_SIGNAL_ID = registerSignal(PowerOn::NAME);
simsignal_t PowerUnit::POWER_OFF_SIGNAL_ID = registerSignal(PowerOff::NAME);
simsignal_t PowerUnit::POWER_UPDATED_SIGNAL_ID =
        registerSignal(PowerUpdated::NAME);

PowerUnit::~PowerUnit()
{
  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->unsubscribe(i->first, this);
}

void PowerUnit::powerOff()
{
  Enter_Method_Silent("powerOff()");
  if (is_powered)
  {
    power = Power::ZERO;
    is_powered = false;
    PowerOff power_off(device_id);
    emit(POWER_OFF_SIGNAL_ID, &power_off);
    processPowerOff();
  }
}

void PowerUnit::powerOn(const Power& power)
{
  Enter_Method_Silent("powerOn(%s)", power.info().c_str());
  if (!is_powered)
  {
    this->power = power;
    this->is_powered = true;
    PowerOn power_on(device_id, power);
    emit(POWER_ON_SIGNAL_ID, &power_on);
    processPowerOn(power);
  }
}

void PowerUnit::updatePower(const Power& power)
{
  Enter_Method_Silent("udpatePower(%s)", power.info().c_str());
  if (this->is_powered)
  {
    this->power = power;
    PowerUpdated power_updated(device_id, power);
    emit(POWER_UPDATED_SIGNAL_ID, &power_updated);
    processPowerUpdated(power);
  }
}

void PowerUnit::initialize() {
  device_id = static_cast<int>(par("deviceID").longValue());
  power = Power::ZERO;
  is_powered = false;

  auto device = findEnclosingRFIDDevice(this);

  subscriptions.clear();
  subscriptions[DeviceControlUnit::DESTROY_DEVICE_SIGNAL_ID] = device;

  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->subscribe(i->first, this);
}

void PowerUnit::receiveSignal(cComponent *source, simsignal_t signal_id,
                              cObject *obj, cObject *details)
{
  if (signal_id == DeviceControlUnit::DESTROY_DEVICE_SIGNAL_ID)
  {
    auto destroy_device = static_cast<DestroyDevice*>(obj);
    if (destroy_device->device_id = device_id)
      processDestroyDevice(*destroy_device);
  }
}

void PowerUnit::processDestroyDevice(const DestroyDevice& upd)
{
  powerOff();
}

void PowerUnit::processPowerOff()
{
  // nothing done here, could be implemented by derived modules
}

void PowerUnit::processPowerOn(const Power& power)
{
  // nothing done here, could be implemented by derived modules
}

void PowerUnit::processPowerUpdated(const Power& power)
{
  // nothing done here, could be implemented by derived modules
}

}
