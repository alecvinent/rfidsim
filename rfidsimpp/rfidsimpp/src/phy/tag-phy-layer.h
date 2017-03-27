#ifndef RFIDSIMPP_PHY_TAG_PHY_LAYER_H_
#define RFIDSIMPP_PHY_TAG_PHY_LAYER_H_

#include <omnetpp.h>
#include <common/optional.hpp>
#include <phy/phy-layer-base.h>

namespace rfidsim {

class TagPhyLayer : public PhyLayer {
 public:
  enum State {
    OFF,
    READY,
    TX,
    RX, WAIT_IR, WAIT_DR
  };
  static const char *str(State state);

  virtual ~TagPhyLayer();

  virtual bool isOn() const { return state != OFF; }
  virtual bool isOff() const { return state == OFF; }

 protected:
  virtual void initialize();
  virtual void handleMessage(omnetpp::cMessage *msg);

  virtual void processPowerOn(const PowerOn& signal);
  virtual void processPowerOff(const PowerOff& signal);
  virtual void processTimeout(omnetpp::cMessage *msg);
  virtual void processRecvBeginInd(RecvBeginInd *msg);
  virtual void processRecvDataInd(RecvDataInd *msg);
  virtual void processRecvErrorInd(RecvErrorInd *msg);
  virtual void processTxFinish(const TxFinish& signal);
  virtual void processReply(epcstd::Reply *msg);

 private:
  void reset();

  nonstd::optional<epcstd::TagEncoding> encoding  = nonstd::nullopt;
  nonstd::optional<epcstd::DivideRatio> divide_ratio = nonstd::nullopt;
  nonstd::optional<bool> use_extended_preamble = nonstd::nullopt;
  nonstd::optional<omnetpp::simtime_t> trcal = nonstd::nullopt;
  nonstd::optional<double> bitrate = nonstd::nullopt;
  nonstd::optional<double> blf = nonstd::nullopt;
  epcstd::Command *recv_cmd = nullptr;

  omnetpp::cMessage *timer = nullptr;
  State state = OFF;

  epcstd::LinkTimingHelper link_timing;

  void setState(State state);
};

}

#endif
