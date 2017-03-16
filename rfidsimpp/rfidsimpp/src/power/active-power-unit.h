#ifndef RFIDSIMPP_POWER_ACTIVE_POWER_UNIT_H_
#define RFIDSIMPP_POWER_ACTIVE_POWER_UNIT_H_

#include <power/power-unit-base.h>

namespace rfidsim {

class ActivePowerUnit : public PowerUnit {
 public:
  virtual ~ActivePowerUnit();

  omnetpp::simtime_t getPowerOnDuration() const { return power_on_duration; }
  omnetpp::simtime_t getPowerOffDuration() const { return power_off_duration; }
  const Power& getMaxPower() const { return max_power; }

 protected:
  virtual void initialize();

  virtual void handleMessage(omnetpp::cMessage *msg);

  virtual void processTimeout(omnetpp::cMessage *timer);

 private:
  omnetpp::simtime_t power_on_duration = omnetpp::SimTime::ZERO;
  omnetpp::simtime_t power_off_duration = omnetpp::SimTime::ZERO;
  Power max_power = Power::ZERO;

  omnetpp::cMessage *timer = nullptr;
};

}

#endif
