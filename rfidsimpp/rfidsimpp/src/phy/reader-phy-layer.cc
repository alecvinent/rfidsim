#include <phy/reader-phy-layer.h>
#include <radio/transceivers/transceiver-base.h>
#include <common/module-access.h>
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
  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->unsubscribe(i->first, this);
}

void ReaderPhyLayer::initialize()
{
  PhyLayer::initialize();

  delimiter = SimTime(par("delimiter"));
  tari = SimTime(par("tari"));
  rtcal = SimTime(par("rtcal"));
  trcal = SimTime(par("trcal"));
  frt = par("frt").doubleValue();

  link_timing.setRTcal(rtcal);
  link_timing.setFrT(frt);
  link_timing.unsetTpri();

  auto device = getEnclosingRFIDDevice(this);
  subscriptions.clear();
  subscriptions[Transmitter::TX_FINISH_SIGNAL_ID] = device;

  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->subscribe(i->first, this);

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
  delete msg;
}

void ReaderPhyLayer::processRecvErrorInd(RecvErrorInd *msg)
{
  recv_reply = nullptr;
  recv_error = static_cast<RecvErrorType>(msg->getType());
  auto t2 = link_timing.getMinT(2);
  scheduleAt(simTime() + t2, timer);
  setState(WAIT_READY_IR_ERROR);
}

void ReaderPhyLayer::processCommand(epcstd::Command *msg)
{
  if (state == READY)
  {
    if (msg->getKind() == epcstd::KIND_COMMAND_QUERY)
    {
      auto query = static_cast<epcstd::Query*>(msg);
      auto dr = static_cast<epcstd::DivideRatio>(query->getDR());
      auto blf = epcstd::getBLF(trcal, dr);
      auto t_pri = epcstd::getTpri(blf);
      link_timing.setTpri(t_pri);
    }
    auto req = new SendCommandReq;
    req->encapsulate(msg);
    send(req, getRadioOut());
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
