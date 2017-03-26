#ifndef RFIDSIMPP_PHY_PHY_LAYER_BASE_H_
#define RFIDSIMPP_PHY_PHY_LAYER_BASE_H_

#include <omnetpp.h>
#include <phy/phy-layer-messages.h>
#include <protocol/epcstd-commands.h>
#include <protocol/epcstd-responses.h>
#include <radio/transceivers/transceiver-messages.h>
#include <power/power-unit-signals.h>

namespace rfidsim {

class PhyLayer : public omnetpp::cSimpleModule, public omnetpp::cListener {
 public:
  virtual ~PhyLayer();

  omnetpp::cGate *getLogicOut() { return gate("logicOut"); }
  omnetpp::cGate *getRadioOut() { return gate("radioOut"); }

  virtual bool isOn() const = 0;
  virtual bool isOff() const = 0;

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
  virtual void processRecvBeginInd(RecvBeginInd *msg) = 0;
  virtual void processRecvDataInd(RecvDataInd *msg) = 0;
  virtual void processRecvErrorInd(RecvErrorInd *msg) = 0;

 private:
  int device_id = -1;
  std::map<omnetpp::simsignal_t, omnetpp::cModule*> subscriptions;
};

}

#endif
