#include <phy/tag-phy-layer.h>
#include <protocol/epcstd-encoder.h>

using namespace omnetpp;

namespace rfidsim {

Define_Module(TagPhyLayer);

const char *TagPhyLayer::str(State state)
{
  switch (state)
  {
    case OFF: return "OFF";
    case READY: return "READY";
    case TX: return "TX";
    case RX: return "RX";
    case WAIT_IR: return "WAIT_IR";
    case WAIT_DR: return "WAIT_DR";
    default: throw cRuntimeError("unrecognized TagPhyLayer::State = %d", state);
  }
}

TagPhyLayer::~TagPhyLayer()
{
  cancelAndDelete(timer);
}

void TagPhyLayer::initialize()
{
  PhyLayer::initialize();
  char buf[64];
  snprintf(buf, sizeof(buf), "TagPHY-%d-timer", getDeviceID());
  timer = new cMessage(buf);

  reset();

  state = OFF;
}

void TagPhyLayer::handleMessage(cMessage *msg)
{
  if (dynamic_cast<epcstd::Reply*>(msg))
    processReply(static_cast<epcstd::Reply*>(msg));
  else
    PhyLayer::handleMessage(msg);
}

void TagPhyLayer::processPowerOn(const PowerOn& signal)
{
  if (state == OFF)
  {
    setState(READY);
  }
}

void TagPhyLayer::processPowerOff(const PowerOff& signal)
{
  if (state != OFF)
  {
    reset();
    setState(OFF);
  }
}

void TagPhyLayer::processTimeout(cMessage *msg)
{
  if (state == WAIT_DR || state == WAIT_IR)
  {
    ASSERT2(recv_cmd, "no received command in state WAIT_DR or WAIT_IR");
    send(recv_cmd, getLogicOut());
    recv_cmd = nullptr;
    setState(READY);
  }
  else
  {
    throw cRuntimeError("unexpected timeout in state %s", str(state));
  }
}

void TagPhyLayer::processRecvBeginInd(RecvBeginInd *msg)
{
  if (state == READY)
  {
    setState(RX);
  }
  delete msg;
}

void TagPhyLayer::processRecvDataInd(RecvDataInd *msg)
{
  if (state == RX)
  {
    ASSERT2(!recv_cmd, "has previously received command when received another");
    recv_cmd = check_and_cast<epcstd::Command*>(msg->decapsulate());
    auto cmd_kind = static_cast<epcstd::CommandKind>(recv_cmd->getKind());

    if (cmd_kind == epcstd::KIND_COMMAND_QUERY)
    {
      auto ind = static_cast<RecvCommandInd*>(msg);
      auto query = static_cast<epcstd::Query*>(recv_cmd);
      auto preamble = ind->getPreamble();

      ASSERT2(preamble.getType() == epcstd::ReaderPreamble::PREAMBLE,
              "Query RecvDataInd carries SYNC instead of PREAMBLE");

      encoding = static_cast<epcstd::TagEncoding>(query->getM());
      divide_ratio = static_cast<epcstd::DivideRatio>(query->getDR());
      use_extended_preamble = query->getTRext();
      trcal = preamble.getTRcal();
      bitrate = epcstd::getTagBitRate(*trcal, *divide_ratio, *encoding);
      blf = epcstd::getBLF(*trcal, *divide_ratio);

      link_timing.setRTcal(preamble.getRTcal());
      link_timing.setTpri(epcstd::getTpri(*blf));
      link_timing.setFrT(epcstd::getFrT(*divide_ratio, *trcal,
                                        getExtendedTemp()));
    }

    auto type = epcstd::getCommandReplyType(cmd_kind);
    if (type == epcstd::IMMEDIATE_REPLY)
    {
      scheduleAt(simTime() + link_timing.getMinT(1), timer);
      setState(WAIT_IR);
    }
    else if (type == epcstd::DELAYED_REPLY)
    {
      scheduleAt(simTime() + link_timing.getMinT(5), timer);
      setState(WAIT_DR);
    }
    else
    {
      throw cRuntimeError("unsupported reply type '%s'", epcstd::str(type));
    }
  }
  else
  {
    throw cRuntimeError("unexpected message '%s' in state %s",
                        msg->getFullName(), str(state));
  }
  delete msg;
}

void TagPhyLayer::processRecvErrorInd(RecvErrorInd *msg)
{
  if (state == RX)
  {
    setState(READY);  // silently discard the received error, no need to inform
                      // logic layer here
  }
  else
  {
    throw cRuntimeError("unexpected message '%s' in state %s",
                        msg->getFullName(), str(state));
  }
  delete msg;
}

void TagPhyLayer::processTxFinish(const TxFinish& signal)
{
  if (state == TX)
  {
    setState(READY);
  }
  else
  {
    throw cRuntimeError("unexpected TxFinish in state %s", str(state));
  }
}

void TagPhyLayer::processReply(epcstd::Reply *msg)
{
  if (state == READY)
  {
    auto preamble = epcstd::TagPreamble();
    preamble.setDivideRatio(*divide_ratio);
    preamble.setTRcal(*trcal);
    preamble.setTRext(*use_extended_preamble);
    preamble.setTagEncoding(*encoding);

    auto req = new SendReplyReq;
    req->setPreamble(preamble);
    req->setPreambleDuration(preamble.getDuration());

    auto body_bitlen = epcstd::getBitLen(msg);
    req->setBodyBitLength(body_bitlen);
    req->setBodyDuration(body_bitlen / *bitrate);

    send(req, getRadioOut());
    setState(TX);
  }
  else
  {
    throw cRuntimeError("unexpected '%s' in state %s", msg->getFullName(),
                        str(state));
  }
}

void TagPhyLayer::reset()
{
  encoding = nonstd::nullopt;
  divide_ratio = nonstd::nullopt;
  use_extended_preamble = nonstd::nullopt;
  trcal = nonstd::nullopt;
  bitrate = nonstd::nullopt;
  blf = nonstd::nullopt;

  link_timing.unsetFrT();
  link_timing.unsetRTcal();
  link_timing.unsetTpri();

  if (timer->isScheduled())
    cancelEvent(timer);
  if (recv_cmd)
    delete recv_cmd;
}

void TagPhyLayer::setState(State state)
{
  if (this->state != state)
  {
    this->state = state;
  }
}

}
