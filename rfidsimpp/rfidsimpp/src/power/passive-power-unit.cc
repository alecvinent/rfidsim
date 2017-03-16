#include <power/passive-power-unit.h>
#include <radio/antenna-base.h>
#include <common/module-access.h>

using namespace omnetpp;

namespace rfidsim {

Define_Module(PassivePowerUnit);

PassivePowerUnit::~PassivePowerUnit()
{
  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->unsubscribe(i->first, this);
}

void PassivePowerUnit::initialize()
{
  PowerUnit::initialize();
  sensitivity = Power(par("sensitivity").doubleValue(), Power::DBM);
  enabled = true;
  peer_power.clear();
  subscriptions.clear();

  auto device = findEnclosingRFIDDevice(this);
  subscriptions[Antenna::RECEIVED_FIELD_OFF_SIGNAL_ID] = device;
  subscriptions[Antenna::RECEIVED_FIELD_ON_SIGNAL_ID] = device;
  subscriptions[Antenna::RECEIVED_FIELD_UPDATED_SIGNAL_ID] = device;

  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->subscribe(i->first, this);
}

void PassivePowerUnit::receiveSignal(
        cComponent *source, simsignal_t signal_id,
        cObject *obj, cObject *details)
{
  if (signal_id == Antenna::RECEIVED_FIELD_OFF_SIGNAL_ID)
  {
    auto received_field_off = static_cast<ReceivedFieldOff*>(obj);
    if (enabled && received_field_off->receiver_device_id == getDeviceID())
      processReceivedFieldOff(*received_field_off);
  }
  else if (signal_id == Antenna::RECEIVED_FIELD_ON_SIGNAL_ID)
  {
    auto received_field_on = static_cast<ReceivedFieldOn*>(obj);
    if (enabled && received_field_on->device_id == getDeviceID())
      processReceivedFieldOn(*received_field_on);
  }
  else if (signal_id == Antenna::RECEIVED_FIELD_UPDATED_SIGNAL_ID)
  {
    auto received_field_updated = static_cast<ReceivedFieldUpdated*>(obj);
    if (enabled && received_field_updated->device_id == getDeviceID())
      processReceivedFieldUpdated(*received_field_updated);
  }
  else
  {
    PowerUnit::receiveSignal(source, signal_id, obj, details);
  }
}

void PassivePowerUnit::processReceivedFieldOff(const ReceivedFieldOff& upd)
{
  int peer_id = upd.peer_device_id;
  auto i = peer_power.find(peer_id);
  if (i != peer_power.end())
  {
    peer_power.erase(i);
    setPower(getTotalPower());
  }
}

void PassivePowerUnit::processReceivedFieldOn(const ReceivedFieldOn& upd)
{
  int peer_id = upd.peer_antenna->getDeviceID();
  peer_power[peer_id] = upd.power;
  setPower(getTotalPower());
}

void PassivePowerUnit::processReceivedFieldUpdated(
        const ReceivedFieldUpdated& upd)
{
  int peer_id = upd.peer_antenna->getDeviceID();
  peer_power[peer_id] = upd.power;
  setPower(getTotalPower());
}

void PassivePowerUnit::processDestroyDevice(const DestroyDevice& upd)
{
  if (enabled)
  {
    PowerUnit::processDestroyDevice(upd);
    enabled = false;
  }
}

Power PassivePowerUnit::getTotalPower() const
{
  Power power = Power::ZERO;
  for (auto i = peer_power.begin(); i != peer_power.end(); ++i)
    power = power.add(i->second);
  return power;
}

void PassivePowerUnit::setPower(const Power& power)
{
  if (!enabled) return;
  if (power < sensitivity && isOn())
  {
    powerOff();
  }
  else if (power >= sensitivity)
  {
    if (isOn())
      updatePower(power);
    else
      powerOn(power);
  }
}


}
