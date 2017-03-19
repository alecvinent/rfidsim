#ifndef RADIO_TRANSCEIVERS_TRANSCEIVER_BASE_H_
#define RADIO_TRANSCEIVERS_TRANSCEIVER_BASE_H_

#include <omnetpp.h>

#include <radio/ber/channel-models.h>

#include <radio/transceivers/transceiver-signals.h>
#include <radio/antenna-signals.h>
#include <power/power-unit-signals.h>
#include <topology/connection-manager-signals.h>

#include <radio/transceivers/transceiver-messages.h>

namespace rfidsim {

class Medium;

class Transmitter : public omnetpp::cSimpleModule, public omnetpp::cListener {
 public:
  enum State { OFF, READY, TX };

  static const char *str(State state);

  static omnetpp::simsignal_t TX_START_SIGNAL_ID;
  static omnetpp::simsignal_t TX_FINISH_SIGNAL_ID;

  virtual ~Transmitter();

  bool isOn() const { return state != OFF; }
  bool isOff() const { return state == OFF; }

  int getDeviceID() const { return device_id; }

  Medium *getMedium() const { return medium; }

 protected:
  virtual void initialize();

  virtual void handleMessage(omnetpp::cMessage *msg);

  virtual void receiveSignal(
          omnetpp::cComponent *source, omnetpp::simsignal_t signal_id,
          omnetpp::cObject *obj, omnetpp::cObject *details);

  virtual void processTimeout(omnetpp::cMessage *timeout);
  virtual void processSendDataReq(SendDataReq *request);
  virtual void processPowerOn(const PowerOn& update);
  virtual void processPowerOff(const PowerOff& update);
  virtual void processReceivedFieldOff(const ReceivedFieldOff& update);
  virtual void processReceivedFieldOn(const ReceivedFieldOn& updated);
  virtual void processReceivedFieldUpdated(const ReceivedFieldUpdated& update);
  virtual void processConnectionCreated(const ConnectionCreated& update);
  virtual void processConnectionLost(const ConnectionLost& update);

  virtual AirFrame *buildAirFrame(SendDataReq *request) = 0;

 private:
  int device_id;
  State state;

  struct Peer {
    int device_id;
    omnetpp::cGate *radio_inp;
    Antenna *active_antenna;
    bool enabled;
  };
  std::map<int, Peer> peers;
  omnetpp::cMessage *tx_timer = nullptr;  // a transmission timeout
  Medium *medium = nullptr;
  Antenna *active_antenna = nullptr;

  std::map<omnetpp::simsignal_t, omnetpp::cModule*> subscriptions;

  void setState(State state);
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

  virtual void processPowerOn(const PowerOn& update);
  virtual void processPowerOff(const PowerOff& update);
  virtual void processTimeout(omnetpp::cMessage *timeout);
  virtual void processAirFrame(AirFrame *air_frame);
  virtual void processTxStart(const TxStart& update);
  virtual void processTxFinish(const TxFinish& update);
  virtual void processReceivedFieldOff(const ReceivedFieldOff& update);
  virtual void processReceivedFieldOn(const ReceivedFieldOn& update);
  virtual void processReceivedFieldUpdate(const ReceivedFieldUpdated& update);
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
