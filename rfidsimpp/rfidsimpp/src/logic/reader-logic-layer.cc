#include <logic/reader-logic-layer.h>
#include <logic/reader-logic-layer-signals.h>
#include <functional>
#include <common/functions.h>

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
    case INVENTORY: return "INVENTORY";
    case ACCESS: return "ACCESS";
    default: throw cRuntimeError("unrecognized ReaderLogicLayer state = %d",
                                 state);
  }
}

ReaderLogicLayer::~ReaderLogicLayer()
{
  delete controller;
  round_man->stop();
  delete round_man;
  cancelAndDelete(launch_msg);
}

void ReaderLogicLayer::initialize()
{
  using namespace std::placeholders;
  LogicLayer::initialize();

  auto ack_cb = std::bind(&ReaderLogicLayer::processTagAck, this,
                          _1, _2, _3);
  auto data_cb = std::bind(&ReaderLogicLayer::processTagData, this, _1);
  auto success_cb = std::bind(&ReaderLogicLayer::processTagSuccess, this);
  auto error_cb = std::bind(&ReaderLogicLayer::processTagError, this, _1);
  round_man = new ReaderRoundManager(this, ack_cb, data_cb, success_cb,
                                     error_cb);

  launch_msg = new cMessage("reader-logic-layer-launch");

  // Initializing controller
  controller = check_and_cast<reader::logic::Controller*>(
          createOne(par("controller").stringValue()));
  auto controller_settings = par("controllerSettings").xmlValue();
  controller->setUp(this, controller_settings);

  // Loading parameters
  auto i_session = static_cast<int>(par("session").longValue());
  if (i_session == 0)
    round_descriptor.session = epcstd::SESSION_0;
  else if (i_session == 1)
    round_descriptor.session = epcstd::SESSION_1;
  else if (i_session == 2)
    round_descriptor.session = epcstd::SESSION_2;
  else if (i_session == 3)
    round_descriptor.session = epcstd::SESSION_3;
  else
    throw cRuntimeError("unrecognized session value = %d", i_session);

  auto i_sel = static_cast<int>(par("sel").longValue());
  if (i_sel == 0)
    round_descriptor.sel = epcstd::SEL_NOT;
  else if (i_sel == 1)
    round_descriptor.sel = epcstd::SEL_YES;
  else if (i_sel == 2)
    round_descriptor.sel = epcstd::SEL_ALL;
  else
    throw cRuntimeError("unrecognized sel value = %d", i_sel);

  auto s_target = upper(trim(par("target").stdstringValue()));
  if (s_target == "A")
    round_descriptor.target = epcstd::FLAG_A;
  else if (s_target == "B")
    round_descriptor.target = epcstd::FLAG_B;
  else
    throw cRuntimeError("unrecognized target value = '%s'", s_target.c_str());

  auto i_q = static_cast<int>(par("q").longValue());
  if (i_q >= 0 && i_q < 16)
    round_descriptor.q = i_q;
  else
    throw cRuntimeError("out-of-bounds Q value = %d", i_q);

  auto s_m = upper(trim(par("m").stdstringValue()));
  if (s_m == "FM0")
    round_descriptor.m = epcstd::FM_0;
  else if (s_m == "M2" || s_m == "MILLER2")
    round_descriptor.m = epcstd::MILLER_2;
  else if (s_m == "M4" || s_m == "MILLER4")
    round_descriptor.m = epcstd::MILLER_4;
  else if (s_m == "M8" || s_m == "MILLER8")
    round_descriptor.m = epcstd::MILLER_8;
  else
    throw cRuntimeError("unrecognized M value = '%s'", par("m").stringValue());

  auto s_dr = trim(par("dr").stdstringValue());
  if (s_dr == "8")
    round_descriptor.dr = epcstd::DR_8;
  else if (s_dr == "64/3")
    round_descriptor.dr = epcstd::DR_64_3;
  else
    throw cRuntimeError("unrecognized DR value = '%s'",
                        par("dr").stringValue());

  round_descriptor.trext = par("trext").boolValue();
}

void ReaderLogicLayer::handleMessage(cMessage *msg)
{
  if (dynamic_cast<PhyDataConf*>(msg))
    round_man->handlePhyDataConf(check_and_cast<PhyDataConf*>(msg));
  else if (dynamic_cast<TagConnReq*>(msg))
    processTagConnReq(check_and_cast<TagConnReq*>(msg));
  else if (dynamic_cast<CloseTagConn*>(msg))
    processCloseTagConn(check_and_cast<CloseTagConn*>(msg));
  else
  {
    LogicLayer::handleMessage(msg);
  }
}

void ReaderLogicLayer::processPowerOn(const PowerOn& signal)
{
  setState(INVENTORY);
  scheduleAt(simTime(), launch_msg);
}

void ReaderLogicLayer::processPowerOff(const PowerOff& signal)
{
  setState(OFF);
  round_man->stop();
}

void ReaderLogicLayer::processTimeout(cMessage *msg)
{
  if (msg == launch_msg)
  {
    startRound();
  }
}

void ReaderLogicLayer::processCloseTagConn(CloseTagConn *msg)
{
  if (state != OFF) {
    startNextSlot();
  }
  delete msg;
}

void ReaderLogicLayer::processTagConnReq(TagConnReq *msg)
{
  if (state != OFF) {
    auto tagop = check_and_cast<tagop::Request*>(msg->decapsulate());
    auto kind = tagop->getKind();
    if (kind == tagop::KIND_ACK)
      processAcknowledgeTagOp(check_and_cast<tagop::Acknowledge*>(tagop));
    else if (kind == tagop::KIND_READ_BANK)
      processReadBankTagOp(check_and_cast<tagop::ReadBank*>(tagop));
    else
      throw cRuntimeError("unrecognized tagop::Request kind = %d", kind);
  }
  delete msg;
}

//
//---------------------------------------------------------------------------
// Tag Operations processing
//---------------------------------------------------------------------------
//

void ReaderLogicLayer::processAcknowledgeTagOp(tagop::Acknowledge *msg)
{
  ASSERT(state != OFF);
  round_man->acknowledge();
}

void ReaderLogicLayer::processReadBankTagOp(tagop::ReadBank *msg)
{
  ASSERT(state != OFF);
  round_man->readBank(static_cast<epcstd::MemoryBank>(msg->getBank()),
                      msg->getWordPtr(), msg->getWordCount());
}

//
//---------------------------------------------------------------------------
// Reader Round Manager callbacks
//---------------------------------------------------------------------------
//
void ReaderLogicLayer::processTagSuccess()
{
  // nothing should be done here since SELECT is not supported yet
}

void ReaderLogicLayer::processTagError(LogicError error)
{
  // TODO: call controller

}

void ReaderLogicLayer::processTagAck(long tag_id, unsigned pc,
                                         const std::vector<uint8_t>& epc)
{
  auto info = new tagop::TagInfo;
  info->setEPC(epc);
  info->setPC(pc);

  TagConnPkt *pkt = nullptr;

  if (state == INVENTORY) {
    curr_tag_id = tag_id;
    pkt = new NewTagConn;
    setState(ACCESS);
  }
  else if (state == ACCESS) {
    pkt = new TagConnConf;
  }
  else {
    throw cRuntimeError("unexpected TagRead in state=%s", str(state));
  }

  pkt->encapsulate(info);
  send(pkt, "appOut");
}

void ReaderLogicLayer::processTagData(const std::vector<uint8_t>& pc)
{

}

void ReaderLogicLayer::startRound()
{
  RoundDescriptor curr_descr = round_descriptor;
  controller->getParams(curr_descr, &round_descriptor);
  slot_index = 1;
  slots_num = static_cast<int>(pow(2, round_descriptor.q));
  round_man->start(round_descriptor);
}

void ReaderLogicLayer::startNextSlot()
{
  ++slot_index;
  if (slot_index > slots_num)
    startRound();
  else
    round_man->nextSlot();
}

void ReaderLogicLayer::setState(State state)
{
  this->state = state;
}

}
