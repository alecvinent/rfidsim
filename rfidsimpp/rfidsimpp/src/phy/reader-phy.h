#ifndef RFIDSIMPP_PHY_READER_PHY_H_
#define RFIDSIMPP_PHY_READER_PHY_H_

#include <omnetpp.h>
#include <phy/phy-base.h>

namespace rfidsim {

class ReaderPhy : public Phy {
 public:
  virtual ~ReaderPhy();

  omnetpp::simtime_t getDelimiter() const { return delimiter; }
  omnetpp::simtime_t getTari() const { return tari; }
  omnetpp::simtime_t getRTcal() const { return rtcal; }
  omnetpp::simtime_t getTRcal() const { return trcal; }

  void setDelimiter(omnetpp::simtime_t t) { delimiter = t; }
  void setTari(omnetpp::simtime_t t) { tari = t; }
  void setRTcal(omnetpp::simtime_t t) { rtcal = t; }
  void setTRcal(omnetpp::simtime_t t) { trcal = t; }

 protected:
  virtual void initialize();
  virtual void handleMessage(omnetpp::cMessage *msg);

  virtual void processPowerOn(const PowerOn& signal);
  virtual void processPowerOff(const PowerOff& signal);
  virtual void processTimeout(omnetpp::cMessage *msg);
  virtual void processRecvBeginInd(RecvBeginInd *msg);
  virtual void processRecvDataInd(RecvDataInd *msg);
  virtual void processRecvErrorInd(RecvErrorInd *msg);
  virtual void processCommand(epcstd::Command *msg);

 private:
  omnetpp::simtime_t delimiter;
  omnetpp::simtime_t tari;
  omnetpp::simtime_t rtcal;
  omnetpp::simtime_t trcal;
};

}

#endif
