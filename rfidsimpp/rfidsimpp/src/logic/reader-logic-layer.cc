#include <logic/reader-logic-layer.h>
#include <logic/reader-logic-layer-signals.h>

using namespace omnetpp;

namespace rfidsim {

Define_Module(ReaderLogicLayer);

simsignal_t ReaderLogicLayer::TAG_READ_DATA_SIGNAL_ID =
        registerSignal(TagReadData::NAME);

simsignal_t ReaderLogicLayer::NEW_SLOT_SIGNAL_ID =
        registerSignal(NewSlot::NAME);

simsignal_t ReaderLogicLayer::NEW_ROUND_SIGNAL_ID =
        registerSignal(NewRound::NAME);

const char *ReaderLogicLayer::str(State state)
{
  switch (state) {
    case OFF: return "OFF";
    case QUERY: return "QUERY";
    case ACK: return "ACK";
    case REQ_RN: return "REQ_RN";
    case READ_TID: return "READ_TID";
    default: throw cRuntimeError("unrecognized ReaderLogicLayer state = %d",
                                 state);
  }
}

ReaderLogicLayer::~ReaderLogicLayer()
{
  //TODO
}

void ReaderLogicLayer::initialize()
{
  LogicLayer::initialize();
}

void ReaderLogicLayer::handleMessage(cMessage *msg)
{
  if (dynamic_cast<PhyDataConf*>(msg))
  {
    auto conf = static_cast<PhyDataConf*>(msg);
    auto status = static_cast<PhyDataConfStatus>(conf->getStatus());
    if (status == PHY_DATA_CONF_OK)
    {
      auto reply = conf->decapsulate();
      auto kind = reply->getKind();
      if (kind == epcstd::KIND_QUERY_REPLY)
        processQueryReply(check_and_cast<epcstd::QueryReply*>(reply));
      else if (kind == epcstd::KIND_ACK_REPLY)
        processAckReply(check_and_cast<epcstd::AckReply*>(reply));
      else if (kind == epcstd::KIND_REQ_RN_REPLY)
        processReqRNReply(check_and_cast<epcstd::ReqRNReply*>(reply));
      else if (kind == epcstd::KIND_READ_REPLY)
        processReadReply(check_and_cast<epcstd::ReadReply*>(reply));
      else
        throw cRuntimeError("unsupported epcstd::Reply kind = %d", kind);
    }
    else
    {
      processPhyError(status);
    }
    delete msg;
  }
  else
  {
    LogicLayer::handleMessage(msg);
  }
}

void ReaderLogicLayer::processPowerOn(const PowerOn& signal)
{
  //TODO
}

void ReaderLogicLayer::processPowerOff(const PowerOff& signal)
{
  //TODO
}

void ReaderLogicLayer::processTimeout(cMessage *msg)
{
  //TODO
}

void ReaderLogicLayer::processQueryReply(epcstd::QueryReply *msg)
{
  //TODO
}

void ReaderLogicLayer::processAckReply(epcstd::AckReply *msg)
{
  //TODO
}

void ReaderLogicLayer::processReqRNReply(epcstd::ReqRNReply *msg)
{
  //TODO
}

void ReaderLogicLayer::processReadReply(epcstd::ReadReply *msg)
{
  //TODO
}

void ReaderLogicLayer::processPhyError(PhyDataConfStatus error)
{
  //TODO
}

}
