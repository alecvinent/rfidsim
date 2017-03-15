#ifndef DEVICES_CONTROL_DEVICE_CONTROL_UNIT_SIGNALS_H_
#define DEVICES_CONTROL_DEVICE_CONTROL_UNIT_SIGNALS_H_

#include <omnetpp.h>

namespace rfidsim {

class DeviceCreated : public omnetpp::cObject, omnetpp::noncopyable {
 public:
  static const char *NAME;

  int device_id = -1;
  omnetpp::cModule *device = nullptr;

  DeviceCreated() {}

  DeviceCreated(int device_id, omnetpp::cModule *device)
    : device_id(device_id), device(device) {}

  virtual std::string info() const;
};

class DeviceDestroyed : public omnetpp::cObject, omnetpp::noncopyable {
 public:
  static const char *NAME;

  int device_id = -1;

  DeviceDestroyed() {}
  DeviceDestroyed(int device_id) : device_id(device_id) {}

  virtual std::string info() const;
};

}

#endif /* DEVICES_CONTROL_DEVICE_CONTROL_UNIT_SIGNALS_H_ */
