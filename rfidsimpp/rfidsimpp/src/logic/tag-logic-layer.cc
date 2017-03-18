#include <logic/tag-logic-layer.h>
#include <common/functions.h>

using namespace omnetpp;

namespace rfidsim {

Define_Module(TagLogicLayer);

const char *TagLogicLayer::str(State state)
{
  switch (state) {
    case OFF: return "OFF";
    case READY: return "READY";
    case ARBITRATE: return "ARBITRATE";
    case REPLY: return "REPLY";
    case ACKNOWLEDGED: return "ACKNOWLEDGED";
    case SECURED: return "SECURED";
    default: throw cRuntimeError("unrecognized TagLogicLayer state = %d",
                                 state);
  }
}

TagLogicLayer::~TagLogicLayer()
{
  //TODO
}

void TagLogicLayer::setEPC(const std::vector<uint8_t>& epc)
{
  if (epc.size() % 2 == 1)
    throw cRuntimeError("EPC byte size must be even");
  this->epc = epc;
}

void TagLogicLayer::setEPC(const std::string& hexstring)
{
  epc = parseHexString(hexstring);
}

void TagLogicLayer::setTID(const std::vector<uint8_t>& tid)
{
  if (tid.size() % 2 == 1)
    throw cRuntimeError("TID byte size must be even");
  this->tid = tid;
}

void TagLogicLayer::setTID(const std::string& hexstring)
{
  tid = parseHexString(hexstring);
}

void TagLogicLayer::initialize()
{
  LogicLayer::initialize();
  setEPC(par("epc").stdstringValue());
  setTID(par("tid").stdstringValue());
}

void TagLogicLayer::handleMessage(cMessage *msg)
{
  if (dynamic_cast<epcstd::Command*>(msg))
  {
    auto kind = msg->getKind();
    if (kind == epcstd::KIND_COMMAND_QUERY)
      processQuery(static_cast<epcstd::Query*>(msg));
    else if (kind == epcstd::KIND_COMMAND_QUERY_REP)
      processQueryRep(static_cast<epcstd::QueryRep*>(msg));
    else if (kind == epcstd::KIND_COMMAND_REQ_RN)
      processReqRN(static_cast<epcstd::ReqRN*>(msg));
    else if (kind == epcstd::KIND_COMMAND_READ)
      processRead(static_cast<epcstd::Read*>(msg));
    else
      throw cRuntimeError("unsupported Command kind = %d", kind);
  }
  else
  {
    LogicLayer::handleMessage(msg);
  }
}

void TagLogicLayer::processPowerOn(const PowerOn& signal)
{
  //TODO
}

void TagLogicLayer::processPowerOff(const PowerOff& signal)
{
  //TODO
}

void TagLogicLayer::processTimeout(cMessage *msg)
{
  //TODO
}

void TagLogicLayer::processQuery(epcstd::Query *msg)
{
  //TODO
}

void TagLogicLayer::processQueryRep(epcstd::QueryRep *msg)
{
  //TODO
}

void TagLogicLayer::processAck(epcstd::Ack *msg)
{
  //TODO
}

void TagLogicLayer::processReqRN(epcstd::ReqRN *msg)
{
  //TODO
}

void TagLogicLayer::processRead(epcstd::Read *msg)
{
  //TODO
}

}
