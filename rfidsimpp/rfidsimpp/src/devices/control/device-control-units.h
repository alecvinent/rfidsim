#ifndef DEVICES_CONTROL_DEVICE_CONTROL_UNITS_H_
#define DEVICES_CONTROL_DEVICE_CONTROL_UNITS_H_

#include <omnetpp.h>

namespace rfidsim {

class DeviceControlUnit : public omnetpp::cSimpleModule,
                          public omnetpp::cListener {
 public:
  static omnetpp::simsignal_t DEVICE_CREATED_SIGNAL_ID;
  static omnetpp::simsignal_t DEVICE_DESTROYED_SIGNAL_ID;

  virtual ~DeviceControlUnit() {}

  virtual void destroy();

 protected:
  virtual int numInitStages() const;
  virtual void initialize(int stage);

  virtual void receiveSignal(
          omnetpp::cComponent *source, omnetpp::simsignal_t signal_id,
          omnetpp::cObject *obj, omnetpp::cObject *details);

 private:
  int device_id = -1;
};

}

#endif /* DEVICES_CONTROL_DEVICE_CONTROL_UNITS_H_ */
