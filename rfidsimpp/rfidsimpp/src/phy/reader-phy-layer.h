#ifndef RFIDSIMPP_PHY_READER_PHY_LAYER_H_
#define RFIDSIMPP_PHY_READER_PHY_LAYER_H_

#include <omnetpp.h>
#include <phy/phy-layer-base.h>
#include <radio/transceivers/transceiver-signals.h>

namespace rfidsim {

class ReaderPhyLayer : public PhyLayer {
 public:
  enum State {
    OFF,
    READY,
    TX_NR, WAIT_READY_NR,
    TX_IR, WAIT_RESP_IR, RX_IR, WAIT_READY_IR_REPLIED, WAIT_READY_IR_ERROR
  };
  static const char *str(State state);
  static PhyDataConfStatus convertRecvErrorToStatus(RecvErrorType error);

  static omnetpp::simtime_t getT1(omnetpp::simtime_t rtcal,
                                  omnetpp::simtime_t t_pri, double frt);

  static omnetpp::simtime_t getT2(omnetpp::simtime_t t_pri);

  static omnetpp::simtime_t getT3(omnetpp::simtime_t t_pri);

  static omnetpp::simtime_t getT4(omnetpp::simtime_t rtcal);

  virtual ~ReaderPhyLayer();

  omnetpp::simtime_t getDelimiter() const { return delimiter; }
  omnetpp::simtime_t getTari() const { return tari; }
  omnetpp::simtime_t getRTcal() const { return rtcal; }
  omnetpp::simtime_t getTRcal() const { return trcal; }

  void setDelimiter(omnetpp::simtime_t t) { delimiter = t; }
  void setTari(omnetpp::simtime_t t) { tari = t; }
  void setRTcal(omnetpp::simtime_t t) { rtcal = t; }
  void setTRcal(omnetpp::simtime_t t) { trcal = t; }

  virtual bool isOn() const { return state != OFF; }
  virtual bool isOff() const { return state == OFF; }
  State getState() const { return state; }

 protected:
  virtual void initialize();
  virtual void handleMessage(omnetpp::cMessage *msg);

  virtual void processPowerOn(const PowerOn& signal);
  virtual void processPowerOff(const PowerOff& signal);
  virtual void processTimeout(omnetpp::cMessage *msg);
  virtual void processTxFinish(const TxFinish& signal);
  virtual void processRecvBeginInd(RecvBeginInd *msg);
  virtual void processRecvDataInd(RecvDataInd *msg);
  virtual void processRecvErrorInd(RecvErrorInd *msg);
  virtual void processCommand(epcstd::Command *msg);

 private:
  omnetpp::simtime_t delimiter;
  omnetpp::simtime_t tari;
  omnetpp::simtime_t rtcal;
  omnetpp::simtime_t trcal;
  double frt;

  epcstd::LinkTimingHelper link_timing;

  State state = OFF;
  omnetpp::cMessage *timer = nullptr;
  epcstd::Reply *recv_reply = nullptr;
  nonstd::optional<RecvErrorType> recv_error = nonstd::nullopt;

  std::map<omnetpp::simsignal_t, omnetpp::cModule*> subscriptions;

  void setState(State state);
};

}

#endif
