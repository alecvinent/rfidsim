#ifndef RFIDSIMPP_LOGIC_TAG_LOGIC_LAYER_H_
#define RFIDSIMPP_LOGIC_TAG_LOGIC_LAYER_H_

#include <protocol/epcstd-base.h>
#include <logic/logic-layer-base.h>

namespace rfidsim {

class TagLogicLayer : public LogicLayer {
 public:
  enum State { OFF, READY, ARBITRATE, REPLY, ACKNOWLEDGED, SECURED };
  const char *str(State state);

  virtual ~TagLogicLayer();

  const std::vector<uint8_t>& getEPC() const { return epc; }
  const std::vector<uint8_t>& getTID() const { return tid; }
  State getState() const { return state; }

  epcstd::InventoryFlag getSession(unsigned index) const {
    return sessions[index];
  }
  epcstd::InventoryFlag getSession(epcstd::Session session) const {
    return this->sessions[epcstd::getSessionIndex(session)];
  }

  bool getSL() const { return sl; }

  void setEPC(const std::vector<uint8_t>& epc);
  void setEPC(const std::string& hexstring);

  void setTID(const std::vector<uint8_t>& tid);
  void setTID(const std::string& hexstring);

 protected:
  virtual void initialize();

  virtual void handleMessage(omnetpp::cMessage *msg);

  virtual void processPowerOn(const PowerOn& signal);
  virtual void processPowerOff(const PowerOff& signal);
  virtual void processTimeout(omnetpp::cMessage *msg);
  virtual void processQuery(epcstd::Query *msg);
  virtual void processQueryRep(epcstd::QueryRep *msg);
  virtual void processAck(epcstd::Ack *msg);
  virtual void processReqRN(epcstd::ReqRN *msg);
  virtual void processRead(epcstd::Read *msg);

 private:
  std::vector<uint8_t> epc;
  std::vector<uint8_t> tid;

  State state;
  epcstd::InventoryFlag sessions[4];
  bool sl;
};

}

#endif
