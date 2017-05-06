#ifndef RFIDSIMPP_LOGIC_READER_LOGIC_LAYER_H_
#define RFIDSIMPP_LOGIC_READER_LOGIC_LAYER_H_

#include <logic/logic-layer-base.h>
#include <phy/phy-layer-messages.h>
#include <logic/reader-round-manager.h>
#include <logic/reader-logic-sap.h>
#include <logic/reader-tagops.h>
#include <logic/control/reader-logic-controller.h>

namespace rfidsim {

class ReaderLogicLayer : public LogicLayer {
 public:
  static omnetpp::simsignal_t TAG_READ_DATA_SIGNAL_ID;
  static omnetpp::simsignal_t NEW_SLOT_SIGNAL_ID;
  static omnetpp::simsignal_t NEW_ROUND_SIGNAL_ID;

  enum State { OFF, INVENTORY, ACCESS };
  static const char *str(State state);

  virtual ~ReaderLogicLayer();

  omnetpp::cGate *getPhyOut() { return gate("phyOut"); }
  const RoundDescriptor& getRoundDescriptor() const { return round_descriptor; }

 protected:
  virtual void initialize();
  virtual void handleMessage(omnetpp::cMessage *msg);

  virtual void processPowerOn(const PowerOn& signal);
  virtual void processPowerOff(const PowerOff& signal);
  virtual void processTimeout(omnetpp::cMessage *msg);

  virtual void processCloseTagConn(CloseTagConn *msg);
  virtual void processTagConnReq(TagConnReq *msg);

  virtual void processAcknowledgeTagOp(tagop::Acknowledge *msg);
  virtual void processReadBankTagOp(tagop::ReadBank *msg);

  virtual void processTagSuccess();
  virtual void processTagError(LogicError error);
  virtual void processTagAck(long tag_id, unsigned pc,
                             const std::vector<uint8_t>& epc);
  virtual void processTagData(const std::vector<uint8_t>& pc);

  virtual void startRound();
  virtual void startNextSlot();

 private:
  RoundDescriptor round_descriptor;
  reader::logic::Controller *controller = nullptr;
  ReaderRoundManager *round_man = nullptr;

  State state = OFF;

  unsigned slots_num;
  unsigned slot_index;
  unsigned long round_index;
  long curr_tag_id;

  omnetpp::cMessage *launch_msg = nullptr;

  void setState(State state);
};

}

#endif
