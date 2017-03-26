#ifndef RFIDSIMPP_PHY_TAG_PHY_LAYER_H_
#define RFIDSIMPP_PHY_TAG_PHY_LAYER_H_

#include <omnetpp.h>
#include <common/optional.hpp>
#include <phy/phy-layer-base.h>

namespace rfidsim {

class TagPhyLayer : public PhyLayer {
 public:
  virtual ~TagPhyLayer();

  virtual bool isOn() const { return true; } //TODO
  virtual bool isOff() const { return true; } //TODO

 protected:
  virtual void initialize();
  virtual void handleMessage(omnetpp::cMessage *msg);

  virtual void processPowerOn(const PowerOn& signal);
  virtual void processPowerOff(const PowerOff& signal);
  virtual void processTimeout(omnetpp::cMessage *msg);
  virtual void processRecvBeginInd(RecvBeginInd *msg);
  virtual void processRecvDataInd(RecvDataInd *msg);
  virtual void processRecvErrorInd(RecvErrorInd *msg);
  virtual void processReply(epcstd::Reply *msg);

 private:
  void reset();

  nonstd::optional<epcstd::TagEncoding> encoding  = nonstd::nullopt;
  nonstd::optional<epcstd::DivideRatio> divide_ratio = nonstd::nullopt;
  nonstd::optional<bool> use_extended_preamble = nonstd::nullopt;
  nonstd::optional<double> blf = nonstd::nullopt;
  nonstd::optional<double> bitrate = nonstd::nullopt;
};

}

#endif
