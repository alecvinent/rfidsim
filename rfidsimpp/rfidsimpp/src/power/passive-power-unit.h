#ifndef RFIDSIMPP_POWER_PASSIVE_POWER_UNIT_H_
#define RFIDSIMPP_POWER_PASSIVE_POWER_UNIT_H_

#include <antenna-signals.h>
#include <power/power-unit-base.h>

namespace rfidsim {

class PassivePowerUnit : public PowerUnit, omnetpp::cListener {
 public:
  virtual ~PassivePowerUnit() {}

 protected:
  virtual void initialize();

  virtual void receiveSignal(
          omnetpp::cComponent *source, omnetpp::simsignal_t signal_id,
          omnetpp::cObject *obj, omnetpp::cObject *details);

 private:
  Power sensitivity;
  Power power;
  bool is_powered_on;
};

}

#endif
