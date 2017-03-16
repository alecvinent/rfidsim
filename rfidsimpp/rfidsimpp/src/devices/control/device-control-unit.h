#ifndef RFIDSIMPP_DEVICES_CONTROL_DEVICE_CONTROL_UNIT_H_
#define RFIDSIMPP_DEVICES_CONTROL_DEVICE_CONTROL_UNIT_H_

#include <omnetpp.h>
#include <devices/control/device-control-signals.h>
#include <world/life/life-signals.h>

namespace rfidsim {

class DeviceControlUnit : public omnetpp::cSimpleModule,
                          public omnetpp::cListener {
 public:
  static omnetpp::simsignal_t DEVICE_CREATED_SIGNAL_ID;
  static omnetpp::simsignal_t DEVICE_DESTROYED_SIGNAL_ID;
  static omnetpp::simsignal_t DESTROY_DEVICE_SIGNAL_ID;

  virtual ~DeviceControlUnit();

  virtual void destroy();

 protected:
  virtual int numInitStages() const;
  virtual void initialize(int stage);

  virtual void receiveSignal(
          omnetpp::cComponent *source, omnetpp::simsignal_t signal_id,
          omnetpp::cObject *obj, omnetpp::cObject *details);

  void processDestoryThing(const DestroyThing& upd);

 private:
  int device_id = -1;

  bool destroyed = false;
  std::map<omnetpp::simsignal_t, omnetpp::cModule*> subscriptions;
};

}

#endif /* DEVICES_CONTROL_DEVICE_CONTROL_UNIT_H_ */
