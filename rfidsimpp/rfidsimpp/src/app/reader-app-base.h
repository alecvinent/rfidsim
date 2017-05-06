#ifndef RFIDSIMPP_APP_READER_APP_BASE_H_
#define RFIDSIMPP_APP_READER_APP_BASE_H_

#include <omnetpp.h>
#include <logic/reader-logic-sap.h>
#include <power/active-power-unit-sap.h>
#include <devices/control/device-control-signals.h>

namespace rfidsim {

class ReaderAppBase : public omnetpp::cSimpleModule,
                      public omnetpp::cListener {
 public:
  virtual ~ReaderAppBase();

  void turnOn();
  void turnOff();

  int getDeviceID() const { return device_id; }

 protected:
//  virtual void processTagReadInd(TagReadInd *msg) = 0;
//  virtual void processTagData(TagData *msg) = 0;
//  virtual void processTagOpOK()

  virtual void processDeviceCreated(const DeviceCreated& sig);
  virtual void processDestroyDevice(const DestroyDevice& sig);

 private:
  int device_id;
};

}

#endif
