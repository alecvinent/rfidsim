#include <phy/reader-phy-layer.h>
#include <common/module-access.h>
#include <protocol/epcstd-encoder.h>
#include <protocol/epcstd-base.h>

using namespace omnetpp;

namespace rfidsim {

Define_Module(ReaderPhyLayer);

const char *ReaderPhyLayer::str(State state)
{
  switch (state) {
    case OFF: return "OFF";
    case READY: return "READY";
    case TX_NR: return "TX_NR";
    case WAIT_READY_NR: return "WAIT_READY_NR";
    case TX_IR: return "TX_IR";
    case WAIT_RESP_IR: return "WAIT_RESP_IR";
    case RX_IR: return "RX_IR";
    case WAIT_READY_IR_REPLIED: return "WAIT_READY_IR_REPLIED";
    case WAIT_READY_IR_ERROR: return "WAIT_READY_IR_ERROR";
    default: throw cRuntimeError("unrecognized ReaderPhyLayer::State = %d",
                                 state);
  }
}

PhyDataConfStatus ReaderPhyLayer::convertRecvErrorToStatus(RecvErrorType error)
{
  switch (error)
  {
    case RECV_ERROR_COLLISION: return PHY_DATA_CONF_COLLISION;
    case RECV_ERROR_LOW_SNR: return PHY_DATA_CONF_CHANNEL_ERROR;
    default: throw cRuntimeError("unrecognized RecvErrorType = %d", error);
  }
}

ReaderPhyLayer::~ReaderPhyLayer()
{
  cancelAndDelete(timer);
}

void ReaderPhyLayer::initialize()
{
  PhyLayer::initialize();

  delimiter = SimTime(par("delimiter"));
  tari = SimTime(par("tari"));
  rtcal = SimTime(par("rtcal"));
  trcal = SimTime(par("trcal"));

  link_timing.setRTcal(rtcal);
  link_timing.unsetFrT();
  link_timing.unsetTpri();

  state = OFF;
}

void ReaderPhyLayer::handleMessage(cMessage *msg)
{
  if (dynamic_cast<epcstd::Command*>(msg))
    processCommand(static_cast<epcstd::Command*>(msg));
  else
    PhyLayer::handleMessage(msg);
}

void ReaderPhyLayer::processPowerOn(const PowerOn& signal)
{
  if (isOff())
    setState(READY);
}

void ReaderPhyLayer::processPowerOff(const PowerOff& signal)
{
  if (isOn())
  {
    setState(OFF);
    if (timer->isScheduled())
      cancelEvent(timer);
    link_timing.unsetTpri(); // T_pri is computed based on DR from Query
    link_timing.unsetFrT();  // FrT also depends on DR
  }
}

void ReaderPhyLayer::processTxFinish(const TxFinish& signal)
{
  if (state == TX_IR)
  {
    auto t1 = link_timing.getMaxT(1);
    auto t3 = link_timing.getMinT(3);
    scheduleAt(simTime() + t1 + t3, timer);
    setState(WAIT_RESP_IR);
  }
  else if (state == TX_NR)
  {
    auto t4 = link_timing.getMinT(4);
    scheduleAt(simTime() + t4, timer);
    setState(WAIT_READY_NR);
  }
  else if (state != OFF)
  {
    throw cRuntimeError("unexpected TxFinish in state %s", str(state));
  }
}

void ReaderPhyLayer::processTimeout(cMessage *msg)
{
  ASSERT(msg == timer);
  if (state == WAIT_READY_NR)
  {
    auto conf = new PhyDataConf;
    conf->setStatus(PHY_DATA_CONF_OK);
    send(conf, getLogicOut());
    setState(READY);
  }
  else if (state == WAIT_RESP_IR)
  {
    auto conf = new PhyDataConf;
    conf->setStatus(PHY_DATA_CONF_NO_REPLY);
    send(conf, getLogicOut());
    setState(READY);
  }
  else if (state == WAIT_READY_IR_REPLIED)
  {
    ASSERT(recv_reply);
    auto conf = new PhyDataConf;
    conf->setStatus(PHY_DATA_CONF_OK);
    conf->encapsulate(recv_reply);
    recv_reply = nullptr;
    send(conf, getLogicOut());
    setState(READY);
  }
  else if (state == WAIT_READY_IR_ERROR)
  {
    ASSERT(recv_error);
    auto conf = new PhyDataConf;
    conf->setStatus(convertRecvErrorToStatus(*recv_error));
    recv_error = nonstd::nullopt;
    send(conf, getLogicOut());
    setState(READY);
  }
  else
  {
    throw cRuntimeError("unexpected timeout '%s' in state %s",
                        msg->getFullName(), str(state));
  }
}

void ReaderPhyLayer::processRecvBeginInd(RecvBeginInd *msg)
{
  if (state == WAIT_RESP_IR)
  {
    setState(RX_IR);
    cancelEvent(timer);
  }
  delete msg;
}

void ReaderPhyLayer::processRecvDataInd(RecvDataInd *msg)
{
  if (state == RX_IR)
  {
    recv_reply = check_and_cast<epcstd::Reply*>(msg->decapsulate());
    recv_error = nonstd::nullopt;
    auto t2 = link_timing.getMinT(2);
    scheduleAt(simTime() + t2, timer);
    setState(WAIT_READY_IR_REPLIED);
  }
  else
  {
    throw cRuntimeError("unexpected message '%s' in state %s",
                        msg->getFullName(), str(state));
  }
  delete msg;
}

void ReaderPhyLayer::processRecvErrorInd(RecvErrorInd *msg)
{
  if (state == RX_IR)
  {
    recv_reply = nullptr;
    recv_error = static_cast<RecvErrorType>(msg->getType());
    auto t2 = link_timing.getMinT(2);
    scheduleAt(simTime() + t2, timer);
    setState(WAIT_READY_IR_ERROR);
  }
  else
  {
    throw cRuntimeError("unexpected message '%s' in state %s",
                        msg->getFullName(), str(state));
  }
  delete msg;
}

void ReaderPhyLayer::processCommand(epcstd::Command *msg)
{
  if (state == READY)
  {
    epcstd::ReaderPreamble preamble;
    preamble.setDelim(delimiter);
    preamble.setTari(tari);
    preamble.setRTcal(rtcal);

    if (msg->getKind() == epcstd::KIND_COMMAND_QUERY)
    {
      auto query = static_cast<epcstd::Query*>(msg);
      dr = static_cast<epcstd::DivideRatio>(query->getDR());
      auto blf = epcstd::getBLF(trcal, *dr);
      auto t_pri = epcstd::getTpri(blf);

      link_timing.setTpri(t_pri);
      link_timing.setFrT(epcstd::getFrT(*dr, trcal, getExtendedTemp()));

      preamble.setTRcal(trcal);
      preamble.setType(epcstd::ReaderPreamble::PREAMBLE);
    }
    else
    {
      preamble.setType(epcstd::ReaderPreamble::SYNC);
    }

    // Encoding command
    char buf[MAX_COMMAND_BITLEN];
    buf[MAX_COMMAND_BITLEN - 1] = '\0';
    unsigned body_bitlen = epcstd::encode(msg, buf, MAX_COMMAND_BITLEN - 1);

    unsigned n0s = 0;
    unsigned n1s = 0;
    epcstd::countBits(buf, &n0s, &n1s);
    auto body_duration = n0s * preamble.getData0() + n1s * preamble.getData1();

    auto req = new SendCommandReq;
    req->setPreamble(preamble);
    req->setPreambleDuration(preamble.getDuration());
    req->setBodyDuration(body_duration);
    req->setBodyBitLength(body_bitlen);
    req->setCommandKind(msg->getKind());
    req->encapsulate(msg);

    send(req, getRadioOut());

    auto type = epcstd::getCommandReplyType(
            static_cast<epcstd::CommandKind>(msg->getKind()));
    if (type == epcstd::NO_REPLY)
      setState(TX_NR);
    else if (type == epcstd::IMMEDIATE_REPLY)
      setState(TX_IR);
    else
      throw cRuntimeError("commands with reply type '%s' not supported",
                          epcstd::str(type));

  }
  else
  {
    throw cRuntimeError("can not process command in state %s", str(state));
  }
}

void ReaderPhyLayer::setState(State state)
{
  if (this->state != state)
  {
    this->state = state;
  }
}

}
