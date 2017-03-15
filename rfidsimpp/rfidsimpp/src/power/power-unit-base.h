#ifndef RFIDSIMPP_RADIO_POWER_POWER_UNIT_BASE_H_
#define RFIDSIMPP_RADIO_POWER_POWER_UNIT_BASE_H_

#include <omnetpp.h>
#include <common/units.h>

namespace rfidsim {

class PowerUnit : public omnetpp::cSimpleModule {
 public:
  static omnetpp::simsignal_t POWER_ON_SIGNAL_ID;
  static omnetpp::simsignal_t POWER_OFF_SIGNAL_ID;
  static omnetpp::simsignal_t POWER_UPDATED_SIGNAL_ID;

  virtual ~PowerUnit() {}

  virtual bool isOn() const = 0;

  virtual bool isOff() const { return !isOn(); }

  virtual Power getPower() const = 0;

  int getDeviceID() const { return device_id; }

 protected:
  virtual void initialize();

  void emitPowerOn(const Power& power);
  void emitPowerOff();
  void emitPowerUpdate(const Power& power);

 private:
  int device_id;
};

}

#endif
