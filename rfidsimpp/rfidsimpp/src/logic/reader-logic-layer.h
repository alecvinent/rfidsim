#ifndef RFIDSIMPP_LOGIC_READER_LOGIC_LAYER_H_
#define RFIDSIMPP_LOGIC_READER_LOGIC_LAYER_H_

#include <logic/logic-layer-base.h>
#include <phy/phy-layer-messages.h>

namespace rfidsim {

class ReaderLogicLayer : public LogicLayer {
 public:
  static omnetpp::simsignal_t TAG_READ_DATA_SIGNAL_ID;
  static omnetpp::simsignal_t NEW_SLOT_SIGNAL_ID;
  static omnetpp::simsignal_t NEW_ROUND_SIGNAL_ID;

  enum State { OFF, QUERY, ACK, REQ_RN, READ_TID };
  static const char *str(State state);

  virtual ~ReaderLogicLayer();

  unsigned getQ() const { return q; }
  epcstd::TagEncoding getM() const { return m; }
  epcstd::DivideRatio getDR() const { return dr; }
  bool getTRext() const { return trext; }
  bool getReadTidBank() const { return read_tid_bank; }

  void setQ(unsigned q) { this->q = q; }
  void setM(epcstd::TagEncoding m) { this->m = m; }
  void setTRext(bool trext) { this->trext = trext; }
  void setReadTIDBank(bool v) { this->read_tid_bank = v; }

 protected:
  virtual void initialize();
  virtual void handleMessage(omnetpp::cMessage *msg);

  virtual void processPowerOn(const PowerOn& signal);
  virtual void processPowerOff(const PowerOff& signal);
  virtual void processTimeout(omnetpp::cMessage *msg);

  virtual void processQueryReply(epcstd::QueryReply *msg);
  virtual void processAckReply(epcstd::AckReply *msg);
  virtual void processReqRNReply(epcstd::ReqRNReply *msg);
  virtual void processReadReply(epcstd::ReadReply *msg);
  virtual void processPhyError(PhyDataConfStatus error);

 private:
  unsigned q;
  epcstd::TagEncoding m;
  epcstd::DivideRatio dr;
  bool trext;
  bool read_tid_bank;

  unsigned handle;
  unsigned slots_num;
  unsigned slot_index;

  unsigned long round_index;
};

}

#endif
