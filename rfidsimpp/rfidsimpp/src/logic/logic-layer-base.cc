#include <logic/logic-layer-base.h>
#include <power/power-unit-base.h>
#include <common/module-access.h>

using namespace omnetpp;

namespace rfidsim {

LogicLayer::~LogicLayer()
{
  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->unsubscribe(i->first, this);
}

void LogicLayer::initialize()
{
  device_id = static_cast<int>(par("deviceID").longValue());
  powered = false;

  auto device = getEnclosingRFIDDevice(this);
  subscriptions.clear();
  subscriptions[PowerUnit::POWER_ON_SIGNAL_ID] = device;
  subscriptions[PowerUnit::POWER_OFF_SIGNAL_ID] = device;
  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->subscribe(i->first, this);
}

void LogicLayer::handleMessage(cMessage *msg)
{
  if (msg->isSelfMessage())
    processTimeout(msg);
  else
    throw cRuntimeError("unexpected message '%s'", msg->getFullName());
}

void LogicLayer::receiveSignal(cComponent *source, simsignal_t signal_id,
                               cObject *obj, cObject *details)
{
  if (signal_id == PowerUnit::POWER_ON_SIGNAL_ID)
  {
    auto signal = check_and_cast<PowerOn*>(obj);
    if (signal->device_id == device_id && !powered)
    {
      powered = true;
      processPowerOn(*signal);
    }
  }
  else if (signal_id == PowerUnit::POWER_OFF_SIGNAL_ID)
  {
    auto signal = check_and_cast<PowerOff*>(obj);
    if (signal->device_id == device_id && powered)
    {
      powered = false;
      processPowerOff(*signal);
    }
  }
}

void LogicLayer::processPowerOn(const PowerOn& signal)
{
  // empty handler
}

void LogicLayer::processPowerOff(const PowerOff& signal)
{
  // empty handler
}

void LogicLayer::processTimeout(cMessage *msg)
{
  // empty handler
}

}
