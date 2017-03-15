#ifndef RADIO_TRANSCEIVERS_TRANSCEIVER_BASE_H_
#define RADIO_TRANSCEIVERS_TRANSCEIVER_BASE_H_

#include <omnetpp.h>
#include <radio/transceivers/transceiver-signals.h>
#include <radio/messages/radio-sap_m.h>
#include <radio/messages/frames_m.h>
#include <radio/ber/channel-models.h>
#include <radio/antenna-signals.h>
#include <power/power-unit-signals.h>
#include <topology/connection-manager-signals.h>

namespace rfidsim {

class Transmitter : public omnetpp::cSimpleModule, public omnetpp::cListener {
 public:
  enum State { OFF, READY, TX };

  static const char *strState(State state);

  static omnetpp::simsignal_t TX_START_SIGNAL_ID;
  static omnetpp::simsignal_t TX_FINISH_SIGNAL_ID;

  virtual ~Transmitter();

 protected:
  virtual void initialize();

  virtual void handleMessage(omnetpp::cMessage *msg);

  virtual void receiveSignal(
          omnetpp::cComponent *source, omnetpp::simsignal_t signal_id,
          omnetpp::cObject *obj, omnetpp::cObject *details);

  virtual void processTimeout(omnetpp::cMessage *timeout);
  virtual void processSendDataReq(SendDataReq *request);
  virtual void processReceivedFieldOff(const ReceivedFieldOff& update);
  virtual void processReceivedFieldOn(const ReceivedFieldOn& updated);
  virtual void processPowerOn(const PowerOn& update);
  virtual void processPowerOff(const PowerOff& update);
  virtual void processConnectionCreated(const ConnectionCreated& update);
  virtual void processConnectionLost(const ConnectionLost& update);

 private:
  int device_id;
  State state;

  struct Peer {
    int device_id;
    omnetpp::cGate *radio_inp;
    bool enabled;
  };
  std::map<int, Peer> peers;

  omnetpp::cMessage *tx_timer = nullptr;  // a transmission timeout

  std::map<omnetpp::simsignal_t, omnetpp::cModule*> subscriptions;
};


class Receiver : public omnetpp::cSimpleModule, public omnetpp::cListener {
 public:
  enum State { OFF, READY, RX, TX };

  virtual ~Receiver();

  ChannelModel *getChannelModel() const;

 protected:
  virtual void initialize();

  virtual void handleMessage(omnetpp::cMessage *msg);

  virtual void receiveSignal(
          omnetpp::cComponent *source, omnetpp::simsignal_t signal_id,
          omnetpp::cObject *obj, omnetpp::cObject *details);

  virtual void processTimeout(omnetpp::cMessage *timeout);
  virtual void processAirFrame(AirFrame *air_frame);
  virtual void processTxStart(const TxStart& update);
  virtual void processTxFinish(const TxFinish& update);
  virtual void processReceivedFieldOff(const ReceivedFieldOff& update);
  virtual void processReceivedFieldOn(const ReceivedFieldOn& updated);
  virtual void processReceivedFieldUpdate(const ReceivedFieldUpdated& update);
  virtual void processPowerOn(const PowerOn& update);
  virtual void processPowerOff(const PowerOff& update);
  virtual void processConnectionCreated(const ConnectionCreated& update);
  virtual void processConnectionLost(const ConnectionLost& update);

 private:
  int device_id;
  State state;

  ChannelModel *channel = nullptr;

  std::map<omnetpp::simsignal_t, omnetpp::cModule*> subscriptions;
};


}

#endif
