#ifndef LOGIC_LOGIC_LAYER_BASE_H_
#define LOGIC_LOGIC_LAYER_BASE_H_

#include <omnetpp.h>
#include <phy/phy-layer-messages.h>
#include <power/power-unit-signals.h>
#include <protocol/epcstd-commands.h>
#include <protocol/epcstd-responses.h>

namespace rfidsim {

struct RoundDescriptor {
  epcstd::Session session;
  epcstd::Sel sel;
  epcstd::InventoryFlag target;
  epcstd::DivideRatio dr;
  epcstd::TagEncoding m;
  bool trext;
  unsigned q;
};

class LogicLayer : public omnetpp::cSimpleModule, public omnetpp::cListener {
 public:
  virtual ~LogicLayer();

  bool isOn() const { return powered; }
  bool isOff() const { return !powered; }

  int getDeviceID() const { return device_id; }

 protected:
  virtual void initialize();

  virtual void handleMessage(omnetpp::cMessage *msg);

  virtual void receiveSignal(
          omnetpp::cComponent *source, omnetpp::simsignal_t signal_id,
          omnetpp::cObject *obj, omnetpp::cObject *details);

  virtual void processPowerOn(const PowerOn& signal);
  virtual void processPowerOff(const PowerOff& signal);
  virtual void processTimeout(omnetpp::cMessage *msg);

 private:
  int device_id = -1;
  bool powered = false;
  std::map<omnetpp::simsignal_t, omnetpp::cModule*> subscriptions;
};

}

#endif
