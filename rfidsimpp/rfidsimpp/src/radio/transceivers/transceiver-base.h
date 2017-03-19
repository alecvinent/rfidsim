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
  double getBandwidth() const { return bandwidth; }

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

  virtual AirFrame *buildAirFrame(SendDataReq *request) const = 0;

 private:
  int device_id;
  double bandwidth;
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
  static const char *str(State state);

  virtual ~Receiver();

  omnetpp::cGate *getPhyOut() { return gate("phyOut"); }

  ChannelModel *getChannelModel() const { return channel; }
  Medium *getMedium() const { return medium; }

  int getDeviceID() const { return device_id; }

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
  virtual void processReceivedFieldUpdated(const ReceivedFieldUpdated& update);
  virtual void processConnectionCreated(const ConnectionCreated& update);
  virtual void processConnectionLost(const ConnectionLost& update);

  /** Returns SNR with respect to frame encoding, preamble length, etc.
   * The `raw_snr` parameter indicates basic signal-to-noise ratio, computed
   * from the received power and doesn't take into account frame details.
   * For the basic implementation, can return `raw_snr` (e.g. for passive
   * receivers).
   */
  virtual double getFrameSNR(AirFrame *frame, double raw_snr) const = 0;

  virtual unsigned getFrameBitLength(AirFrame *frame) const = 0;

  virtual RecvDataInd *buildRecvDataInd(AirFrame *frame) const = 0;

 private:
  int device_id;

  State state;
  ChannelModel *channel = nullptr;
  Medium *medium = nullptr;
  std::map<omnetpp::simsignal_t, omnetpp::cModule*> subscriptions;

  struct Peer {
    int device_id;
    Power power;
  };
  std::map<int, Peer> peers;

  struct RxRecord {
    unsigned long rxop_index = 0;
    int peer_id = -1;
    AirFrame *frame = nullptr;
    Power min_power = Power::ZERO;
    Power max_power = Power::ZERO;
    Power noise = Power::ZERO;
    omnetpp::cMessage *timer = nullptr;
    omnetpp::simtime_t started_at = omnetpp::SimTime::ZERO;
    bool broken = false;
  };

  std::map<int, RxRecord*> rxops_by_peer;
  std::map<omnetpp::cMessage*, RxRecord*> rxops_by_timer;
  std::set<RxRecord*> rxops;
  unsigned long next_rxop_index = 0;

  void setState(State state);
  void updatePeerPower(int peer_id, const Power& power);
  int getPeerID(int reader_id, int tag_id);
};


}

#endif
