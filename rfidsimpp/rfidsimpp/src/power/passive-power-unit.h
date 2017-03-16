#ifndef RFIDSIMPP_POWER_PASSIVE_POWER_UNIT_H_
#define RFIDSIMPP_POWER_PASSIVE_POWER_UNIT_H_

#include <antenna-signals.h>
#include <power/power-unit-base.h>
#include <radio/antenna-signals.h>

namespace rfidsim {

class PassivePowerUnit : public PowerUnit {
 public:
  virtual ~PassivePowerUnit();

  bool isEnabled() const { return enabled; }

 protected:
  virtual void initialize();

  virtual void receiveSignal(
          omnetpp::cComponent *source, omnetpp::simsignal_t signal_id,
          omnetpp::cObject *obj, omnetpp::cObject *details);

  virtual void processReceivedFieldOff(const ReceivedFieldOff& upd);
  virtual void processReceivedFieldOn(const ReceivedFieldOn& upd);
  virtual void processReceivedFieldUpdated(const ReceivedFieldUpdated& upd);
  virtual void processDestroyDevice(const DestroyDevice& upd);

 private:
  Power getTotalPower() const;
  void setPower(const Power& power);

 private:
  Power sensitivity = Power::ZERO;
  bool enabled = true;
  std::map<int, Power> peer_power;
  std::map<omnetpp::simsignal_t, omnetpp::cModule*> subscriptions;
};

}

#endif
