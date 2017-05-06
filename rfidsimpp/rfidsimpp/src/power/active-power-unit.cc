#include <power/active-power-unit.h>

using namespace omnetpp;

namespace rfidsim {

Define_Module(ActivePowerUnit);

ActivePowerUnit::~ActivePowerUnit()
{
  cancelAndDelete(timer);
}

void ActivePowerUnit::initialize()
{
  PowerUnit::initialize();

  power_on_duration = par("powerOnDuration").doubleValue();
  power_off_duration = par("powerOffDuration").doubleValue();
  max_power = Power(par("maxPower").doubleValue(), Power::DBM);

  static char timer_name[32];
  snprintf(timer_name, sizeof(timer_name), "APU-%d-Timer", getDeviceID());
  timer = new omnetpp::cMessage(timer_name);

  scheduleAt(simTime(), timer);
}

void ActivePowerUnit::handleMessage(omnetpp::cMessage *msg)
{
  if (msg->isSelfMessage())
    processTimeout(msg);
  else
  {
    if (dynamic_cast<TurnPowerUnitOn*>(msg))
      processTurnPowerUnitOn(check_and_cast<TurnPowerUnitOn*>(msg));
    else if (dynamic_cast<TurnPowerUnitOff*>(msg))
      processTurnPowerUnitOff(check_and_cast<TurnPowerUnitOff*>(msg));
    else
      throw cRuntimeError("unrecognized message: %s", msg->info().c_str());
  }
}

void ActivePowerUnit::processTimeout(omnetpp::cMessage *timer)
{
  ASSERT(timer == this->timer);
  if (isOn())
  {
    powerOff();
    scheduleAt(simTime() + power_off_duration, timer);
  }
  else
  {
    powerOn(max_power);
    scheduleAt(simTime() + power_on_duration, timer);
  }
}

void ActivePowerUnit::processTurnPowerUnitOn(TurnPowerUnitOn *msg)
{
  if (!turned_on)
  {
    turned_on = true;
    powerOn(max_power);
    scheduleAt(simTime() + power_on_duration, timer);
  }
  delete msg;
}

void ActivePowerUnit::processTurnPowerUnitOff(TurnPowerUnitOff *msg)
{
  if (turned_on)
  {
    turned_on = false;
    cancelEvent(timer);
    if (isOn())
      powerOff();
  }
  delete msg;
}

}
