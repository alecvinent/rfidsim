#ifndef RFIDSIMPP_RADIO_POWER_POWER_UNIT_BASE_H_
#define RFIDSIMPP_RADIO_POWER_POWER_UNIT_BASE_H_

#include <omnetpp.h>
#include <common/units.h>
#include <devices/control/device-control-signals.h>

namespace rfidsim {

class PowerUnit : public omnetpp::cSimpleModule, public omnetpp::cListener {
 public:
  static omnetpp::simsignal_t POWER_ON_SIGNAL_ID;
  static omnetpp::simsignal_t POWER_OFF_SIGNAL_ID;
  static omnetpp::simsignal_t POWER_UPDATED_SIGNAL_ID;

  virtual ~PowerUnit();

  bool isOn() const { return is_powered; }
  bool isOff() const { return !is_powered; }
  const Power& getPower() const { return power; }
  int getDeviceID() const { return device_id; }

  virtual void powerOff();
  virtual void powerOn(const Power& power);
  virtual void updatePower(const Power& power);

 protected:
  virtual void initialize();

  virtual void receiveSignal(
          omnetpp::cComponent *source, omnetpp::simsignal_t signal_id,
          omnetpp::cObject *obj, omnetpp::cObject *details);

  virtual void processDestroyDevice(const DestroyDevice& upd);
  virtual void processPowerOn(const Power& power);
  virtual void processPowerUpdated(const Power& power);
  virtual void processPowerOff();

 private:
  int device_id = -1;
  bool is_powered = false;
  Power power = Power::ZERO;
  std::map<omnetpp::simsignal_t, omnetpp::cModule*> subscriptions;
};

}

#endif
