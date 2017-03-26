#include <phy/phy-layer-base.h>
#include <common/module-access.h>
#include <power/power-unit-base.h>

using namespace omnetpp;

namespace rfidsim {

PhyLayer::~PhyLayer()
{
  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->unsubscribe(i->first, this);
}

void PhyLayer::initialize()
{
  device_id = static_cast<int>(par("deviceID").longValue());

  auto device = getEnclosingRFIDDevice(this);
  subscriptions.clear();
  subscriptions[PowerUnit::POWER_OFF_SIGNAL_ID] = device;
  subscriptions[PowerUnit::POWER_ON_SIGNAL_ID] = device;
  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->subscribe(i->first, this);
}

void PhyLayer::handleMessage(cMessage *msg)
{
  if (msg->isSelfMessage())
    processTimeout(msg);
  else if (dynamic_cast<RecvBeginInd*>(msg))
    processRecvBeginInd(static_cast<RecvBeginInd*>(msg));
  else if (dynamic_cast<RecvDataInd*>(msg))
    processRecvDataInd(static_cast<RecvDataInd*>(msg));
  else if (dynamic_cast<RecvErrorInd*>(msg))
    processRecvErrorInd(static_cast<RecvErrorInd*>(msg));
  else
    throw cRuntimeError("unexpected message '%s'", msg->getFullName());
}

void PhyLayer::receiveSignal(cComponent *source, simsignal_t signal_id,
                             cObject *obj, cObject *details)
{
  if (signal_id == PowerUnit::POWER_OFF_SIGNAL_ID)
  {
    auto signal = static_cast<PowerOff*>(obj);
    if (signal->device_id == device_id)
      processPowerOff(*signal);
  }
  else if (signal_id == PowerUnit::POWER_ON_SIGNAL_ID)
  {
    auto signal = static_cast<PowerOn*>(obj);
    if (signal->device_id == device_id)
      processPowerOn(*signal);
  }
}

void PhyLayer::processPowerOn(const PowerOn& signal)
{
  // empty handler
}

void PhyLayer::processPowerOff(const PowerOff& signal)
{
  // empty handler
}

void PhyLayer::processTimeout(cMessage *msg)
{
  // empty handler
}

}
