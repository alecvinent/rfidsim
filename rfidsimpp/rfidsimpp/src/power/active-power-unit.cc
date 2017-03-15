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

  is_on = false;
  power = Power::ZERO;

  scheduleAt(simTime(), timer);
}

void ActivePowerUnit::handleMessage(omnetpp::cMessage *msg)
{
  if (msg->isSelfMessage())
  {
    processTimer(msg);
  }
  else
  {
    throw cRuntimeError("unrecognized message: %s", msg->info().c_str());
  }
}

void ActivePowerUnit::processTimer(omnetpp::cMessage *timer)
{
  ASSERT(timer == this->timer);
  if (is_on)
  {
    scheduleAt(simTime() + power_off_duration, timer);
    is_on = false;
    power = Power::ZERO;
    emitPowerOff();
  }
  else
  {
    scheduleAt(simTime() + power_on_duration, timer);
    is_on = true;
    power = max_power;
    emitPowerOn(power);
  }
}

}
