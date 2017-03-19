#include <radio/transceivers/transceiver-base.h>
#include <radio/antenna-base.h>
#include <radio/medium/medium.h>
#include <power/power-unit-base.h>
#include <topology/connection-manager.h>
#include <common/module-access.h>

using namespace omnetpp;

namespace rfidsim {

//
//===========================================================================
// Transmitter
//===========================================================================
//
simsignal_t Transmitter::TX_START_SIGNAL_ID = registerSignal(TxStart::NAME);
simsignal_t Transmitter::TX_FINISH_SIGNAL_ID = registerSignal(TxFinish::NAME);

const char *Transmitter::str(State state)
{
  switch (state) {
    case OFF: return "OFF";
    case READY: return "READY";
    case TX: return "TX";
    default: throw cRuntimeError("unrecognized Transmitter state=%d", state);
  }
}

Transmitter::~Transmitter()
{
  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->unsubscribe(i->first, this);
}

void Transmitter::initialize()
{
  device_id = static_cast<int>(par("deviceID").longValue());
  state = OFF;
  peers.clear();
  tx_timer = new cMessage("Transmitter-%d-Timer");
  medium = findChildByType<Medium>(getSystemModule());

  subscriptions.clear();
  auto device = findEnclosingRFIDDevice(this);
  auto sys = getSystemModule();
  subscriptions[PowerUnit::POWER_OFF_SIGNAL_ID] = device;
  subscriptions[PowerUnit::POWER_ON_SIGNAL_ID] = device;
  subscriptions[Antenna::RECEIVED_FIELD_OFF_SIGNAL_ID] = device;
  subscriptions[Antenna::RECEIVED_FIELD_ON_SIGNAL_ID] = device;
  subscriptions[Antenna::RECEIVED_FIELD_UPDATED_SIGNAL_ID] = device;
  subscriptions[ConnectionManager::CONNECTION_CREATED_SIGNAL_ID] = sys;
  subscriptions[ConnectionManager::CONNECTION_LOST_SIGNAL_ID] = sys;
  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->subscribe(i->first, this);
}

void Transmitter::handleMessage(cMessage *msg)
{
  if (msg->isSelfMessage())
    processTimeout(msg);
  else if (dynamic_cast<SendDataReq*>(msg))
    processSendDataReq(static_cast<SendDataReq*>(msg));
  else
    throw cRuntimeError("unexpected message '%s'", msg->getFullName());
}

void Transmitter::receiveSignal(cComponent *source, simsignal_t signal_id,
                                cObject *obj, cObject *details)
{
  if (signal_id == Antenna::RECEIVED_FIELD_OFF_SIGNAL_ID)
  {
    auto signal_ = check_and_cast<ReceivedFieldOff*>(obj);
    if (signal_->receiver_device_id == device_id)
      processReceivedFieldOff(*signal_);
  }
  else if (signal_id == Antenna::RECEIVED_FIELD_ON_SIGNAL_ID)
  {
    auto signal_ = check_and_cast<ReceivedFieldOn*>(obj);
    if (signal_->device_id == device_id)
      processReceivedFieldOn(*signal_);
  }
  else if (signal_id == PowerUnit::POWER_OFF_SIGNAL_ID)
  {
    auto signal_ = check_and_cast<PowerOff*>(obj);
    if (signal_->device_id == device_id)
      processPowerOff(*signal_);
  }
  else if (signal_id == PowerUnit::POWER_ON_SIGNAL_ID)
  {
    auto signal_ = check_and_cast<PowerOn*>(obj);
    if (signal_->device_id == device_id)
      processPowerOn(*signal_);
  }
  else if (signal_id == ConnectionManager::CONNECTION_CREATED_SIGNAL_ID)
  {
    auto signal_ = check_and_cast<ConnectionCreated*>(obj);
    if (signal_->reader_id == device_id || signal_->tag_id == device_id)
      processConnectionCreated(*signal_);
  }
  else if (signal_id == ConnectionManager::CONNECTION_LOST_SIGNAL_ID)
  {
    auto signal_ = check_and_cast<ConnectionLost*>(obj);
    if (signal_->reader_id == device_id || signal_->tag_id == device_id)
      processConnectionLost(*signal_);
  }
}

void Transmitter::processTimeout(cMessage *timeout)
{
  ASSERT(state == TX);
  setState(READY);
}

void Transmitter::processSendDataReq(SendDataReq *request)
{
  if (state == READY)
  {
    auto frame = buildAirFrame(request);
    for (auto i = peers.begin(); i != peers.end(); i++)
    {
      const Peer& peer = i->second;
      if (peer.enabled)
      {
        auto delay = medium->getPropagationDelay(
                active_antenna, peer.active_antenna);
        sendDirect(frame->dup(), delay, 0.0, peer.radio_inp);
      }
    }
    delete frame;

    // Change state to TX
    setState(TX);

    // Informing Receiver that the transmission started
    TxStart signal(device_id);
    emit(TX_START_SIGNAL_ID, &signal);

    // Scheduling the transmission end
    auto duration = request->getPreambleDuration() +
            request->getBodyDuration();
    scheduleAt(simTime() + duration, tx_timer);
  }
  else
  {
    throw cRuntimeError("Transmitter: SendDataReq within state %s", str(state));
  }
}

void Transmitter::processPowerOff(const PowerOff& update)
{
  // During power off, transmission is being cancelled
  if (state != OFF)
  {
    if (state == TX)
      cancelEvent(tx_timer);
    setState(OFF);
  }
}

void Transmitter::processPowerOn(const PowerOn& update)
{
  if (state == OFF)
    setState(READY);
}

void Transmitter::processReceivedFieldOff(const ReceivedFieldOff& update)
{
  // When the peer is turned off, set it with 'disabled' status
  auto i = peers.find(update.peer_device_id);
  if (i != peers.end())
    i->second.enabled = true;
}

void Transmitter::processReceivedFieldOn(const ReceivedFieldOn& update)
{
  // When another device is turned on, and this device is a peer, set its
  // status to 'enabled'. Devices those are not peers, are ignored.
  auto i = peers.find(update.device_id);
  if (i != peers.end())
  {
    i->second.enabled = true;
    i->second.active_antenna = update.peer_antenna;
  }
}

void Transmitter::processReceivedFieldUpdated(
        const ReceivedFieldUpdated& update)
{
  // Upon received field update, update its antenna since it is used for
  // delay computation.
  auto i = peers.find(update.device_id);
  if (i != peers.end())
  {
    i->second.enabled = true; // power update may be received after connection
                              // created, so enable the peer anyway
    i->second.active_antenna = update.peer_antenna;
  }
}

void Transmitter::processConnectionCreated(const ConnectionCreated& update)
{
  int peer_id;
  omnetpp::cModule *peer = nullptr;
  if (device_id == update.reader_id)
  {
    peer_id = update.tag_id;
    peer = update.tag;
  }
  else
  {
    ASSERT(device_id == update.tag_id);
    peer_id = update.reader_id;
    peer = update.reader;
  }
  auto radio_inp = peer->gate("radioInp");

  // Since active antenna will be known after peer power update/on, leave
  // it as nullptr and set peer to disabled state until then
  Peer record = {peer_id, radio_inp, nullptr, false};
  peers[peer_id] = record;
}

void Transmitter::processConnectionLost(const ConnectionLost& update)
{
  int peer_id = update.reader_id == device_id ? update.tag_id :
          update.reader_id;
  peers.erase(peer_id);
}

void Transmitter::setState(State state)
{
  this->state = state;
}



//
//===========================================================================
// Receiver
//===========================================================================
//
const char *Receiver::str(State state)
{
  switch (state) {
    case OFF: return "OFF";
    case READY: return "READY";
    case RX: return "RX";
    case TX: return "TX";
    default: throw cRuntimeError("unrecognized Receiver state = %d", state);
  }
}

Receiver::~Receiver()
{
  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->unsubscribe(i->first, this);
}

void Receiver::initialize()
{
  device_id = static_cast<int>(par("deviceID").longValue());
  state = OFF;
  channel = findChildByType<ChannelModel>(getParentModule());
  medium = findChildByType<Medium>(getSystemModule());

  auto parent = getParentModule();
  auto device = findEnclosingRFIDDevice(this);
  auto sys = getSystemModule();
  subscriptions.clear();
  subscriptions[Transmitter::TX_START_SIGNAL_ID] = parent;
  subscriptions[Transmitter::TX_FINISH_SIGNAL_ID] = parent;
  subscriptions[Antenna::RECEIVED_FIELD_OFF_SIGNAL_ID] = device;
  subscriptions[Antenna::RECEIVED_FIELD_ON_SIGNAL_ID] = device;
  subscriptions[Antenna::RECEIVED_FIELD_UPDATED_SIGNAL_ID] = device;
  subscriptions[PowerUnit::POWER_OFF_SIGNAL_ID] = device;
  subscriptions[PowerUnit::POWER_ON_SIGNAL_ID] = device;
  subscriptions[ConnectionManager::CONNECTION_CREATED_SIGNAL_ID] = sys;
  subscriptions[ConnectionManager::CONNECTION_LOST_SIGNAL_ID] = sys;
  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->subscribe(i->first, this);
}

void Receiver::handleMessage(cMessage *msg)
{
  if (msg->isSelfMessage())
    processTimeout(msg);
  else if (dynamic_cast<AirFrame*>(msg))
    processAirFrame(static_cast<AirFrame*>(msg));
  else
    throw cRuntimeError("unexpected message '%s'", msg->getFullName());
}

void Receiver::receiveSignal(cComponent *source, simsignal_t signal_id,
        cObject *obj, cObject *details)
{
  if (signal_id == Transmitter::TX_START_SIGNAL_ID)
  {
    auto signal_ = static_cast<TxStart*>(obj);
    if (signal_->device_id == device_id)
      processTxStart(*signal_);
  }
  else if (signal_id == Transmitter::TX_FINISH_SIGNAL_ID)
  {
    auto signal_ = static_cast<TxFinish*>(obj);
    if (signal_->device_id == device_id)
      processTxFinish(*signal_);
  }
  else if (signal_id == Antenna::RECEIVED_FIELD_OFF_SIGNAL_ID)
  {
    auto signal_ = static_cast<ReceivedFieldOff*>(obj);
    if (signal_->receiver_device_id == device_id)
      processReceivedFieldOff(*signal_);
  }
  else if (signal_id == Antenna::RECEIVED_FIELD_ON_SIGNAL_ID)
  {
    auto signal_ = static_cast<ReceivedFieldOn*>(obj);
    if (signal_->device_id == device_id)
      processReceivedFieldOn(*signal_);
  }
  else if (signal_id == Antenna::RECEIVED_FIELD_UPDATED_SIGNAL_ID)
  {
    auto signal_ = static_cast<ReceivedFieldUpdated*>(obj);
    if (signal_->device_id == device_id)
      processReceivedFieldUpdated(*signal_);
  }
  else if (signal_id == PowerUnit::POWER_OFF_SIGNAL_ID)
  {
    auto signal_ = static_cast<PowerOff*>(obj);
    if (signal_->device_id == device_id)
      processPowerOff(*signal_);
  }
  else if (signal_id == PowerUnit::POWER_ON_SIGNAL_ID)
  {
    auto signal_ = static_cast<PowerOn*>(obj);
    if (signal_->device_id == device_id)
      processPowerOn(*signal_);
  }
  else if (signal_id == ConnectionManager::CONNECTION_CREATED_SIGNAL_ID)
  {
    auto signal_ = static_cast<ConnectionCreated*>(obj);
    if (signal_->reader_id == device_id || signal_->tag_id == device_id)
      processConnectionCreated(*signal_);
  }
  else if (signal_id == ConnectionManager::CONNECTION_LOST_SIGNAL_ID)
  {
    auto signal_ = static_cast<ConnectionLost*>(obj);
    if (signal_->reader_id == device_id || signal_->tag_id == device_id)
      processConnectionLost(*signal_);
  }
}

void Receiver::processPowerOn(const PowerOn& update)
{
  if (state != OFF)
    return;
  setState(READY);
}

void Receiver::processPowerOff(const PowerOff& update)
{
  // When powering off, mark all RX-ops as broken, so they won't be ever
  // delivered, but will still be treated as noise if some new RX-op will
  // appear after power on and till they won't be finished.
  if (state == OFF)
    return;
  for (auto i = rxops.begin(); i != rxops.end(); ++i)
    (*i)->broken = true;
  setState(OFF);
}

void Receiver::processTimeout(cMessage *timeout)
{
  auto rxop_iter = rxops_by_timer.find(timeout);
  if (rxop_iter != rxops_by_timer.end())
  {
    RxRecord *rxop = rxop_iter->second;

    if (!rxop->broken)
    {
      // If RXOP is not broken, state MUST be RX. After processing this RXOP,
      // the state will be changed to READY, no matter how many other RXOPs are
      // ongoing - they will be treated as noise.
      ASSERT(state == RX);

      // 1) compute SNR
      Power noise = rxop->noise;
      for (auto other_rxop: rxops)
        if (other_rxop != rxop)
          noise = noise.add(other_rxop->max_power);
      double raw_snr = rxop->min_power.watt() / noise.watt();
      double snr = getFrameSNR(rxop->frame, raw_snr);

      // 2) compute BER
      double ber = channel->getBER(snr);

      // 3) decide whether to deliver to the upper layer
      unsigned frame_bit_length = getFrameBitLength(rxop->frame);
      double frame_success_prob = pow(1 - ber, frame_bit_length);
      double p = uniform(0.0, 1.0);

      // 4) delivering the frame or error code
      if (p <= frame_success_prob)
      {
        auto ind = buildRecvDataInd(rxop->frame);
        send(ind, getPhyOut());
      }
      else
      {
        RecvErrorInd *ind = new RecvErrorInd;
        RecvErrorType error_type = rxops.size() > 1 ? RECV_ERROR_COLLISION :
                RECV_ERROR_LOW_SNR;
        ind->setType(error_type);
        send(ind, getPhyOut());
      }

      // 5) changing state to READY, no matter how many there are still
      // ongoing RXOPs are there - they will become noise.
      setState(READY);
    }

    // Since RXOP is finished, adding its maximum power level to all other
    // ongoing RXOPs, so it'll be treated as noise
    for (auto other_rxop: rxops)
      if (other_rxop != rxop)
        other_rxop->noise  = other_rxop->noise.add(rxop->max_power);

    // Removing RXOP
    rxops_by_timer.erase(rxop_iter);
    rxops_by_peer.erase(rxop->peer_id);
    rxops.erase(rxop);
    delete timeout;
    delete rxop->frame;
    delete rxop;
  }
}

void Receiver::processAirFrame(AirFrame *frame)
{
  // Since information about power is available for peers only, searching
  // for the peer by frame source DeviceID. If peer not found, frame is
  // silently removed.
  int peer_id = frame->getDeviceID();
  auto peer_iter = peers.find(peer_id);
  if (peer_iter != peers.end())
  {
    const Power& peer_power = peer_iter->second.power;
    Power frame_power = peer_power.amplify(frame->getModulationLoss());

    // Create and schedule the timer
    char timer_name[64];
    snprintf(timer_name, sizeof(timer_name),
             "RXOP{ReceiverID=%d, SenderID=%d, RXOPID=%lu}-Timer",
             device_id, peer_id, next_rxop_index);
    auto timer = new cMessage(timer_name);
    simtime_t duration = frame->getPreambleDuration() +
            frame->getBodyDuration();
    scheduleAt(simTime() + duration, timer);

    // Create a new RXOP
    RxRecord *rxop = new RxRecord;
    rxop->rxop_index = next_rxop_index++;
    rxop->peer_id = peer_id;
    rxop->frame = frame;
    rxop->min_power = frame_power;
    rxop->max_power = frame_power;
    rxop->noise = medium->getThermalNoise();
    rxop->timer = timer;
    rxop->started_at = simTime();
    rxop->broken = state != READY;

    // Put the RXOP into maps and assert no RXOPs from that peer exist
    ASSERT2(rxops_by_peer.count(peer_id) == 0,
            "Simultaneous RXOPs from the same device");
    rxops.insert(rxop);
    rxops_by_peer[frame->getDeviceID()] = rxop;
    rxops_by_timer[timer] = rxop;
  }
  else
  {
    delete frame;
  }
}

void Receiver::processTxStart(const TxStart& update)
{
  // Assert that TxStart is received only when powered on (otherwise there is
  // an error with PowerOn delivery and processing order)
  ASSERT2(state != OFF, "check PowerOn/PowerOff delivery and processing order");

  // If in RX state, mark all RXOPs as broken
  if (state == RX)
    for (auto rxop: rxops)
      rxop->broken = true;

  setState(TX);
}

void Receiver::processTxFinish(const TxFinish& update)
{
  ASSERT2(state != OFF, "check PowerOn/PowerOff delivery and processing order");
  ASSERT2(state == TX, "state must be TX when receiving TxFinish at Receiver");
  setState(READY);
}

void Receiver::processReceivedFieldOff(const ReceivedFieldOff& update)
{
  // Update RXOP from that peer
  auto rxop_iter = rxops_by_peer.find(update.peer_device_id);
  if (rxop_iter != rxops_by_peer.end())
  {
    auto rxop = rxop_iter->second;

    // If RXOP was the active one (Receiver is in RX state and RXOP is not
    // broken), mark it as broken and move to READY state
    if (!rxop->broken)
    {
      ASSERT(state == RX);
      rxop->broken = true;
      setState(READY);
    }

    // Update RXOP power
    rxop->min_power = Power::ZERO;
  }

  // Update the peer power
  auto peer_iter = peers.find(update.peer_device_id);
  if (peer_iter != peers.end())
  {
    peer_iter->second.power = Power::ZERO;
  }
  else
  {
    ASSERT2(rxop_iter == rxops_by_peer.end(),
            "RXOPs from unknown devices disallowed");
  }
}

void Receiver::processReceivedFieldOn(const ReceivedFieldOn& update)
{
  auto peer_id = update.peer_antenna->getDeviceID();
  auto power = update.power;
  updatePeerPower(peer_id, power);
}

void Receiver::processReceivedFieldUpdated(const ReceivedFieldUpdated& update)
{
  auto peer_id = update.peer_antenna->getDeviceID();
  auto power = update.power;
  updatePeerPower(peer_id, power);
}

void Receiver::processConnectionCreated(const ConnectionCreated& update)
{
  int peer_id = getPeerID(update.reader_id, update.tag_id);
  if (peers.count(peer_id) == 0)
    peers[peer_id] = {peer_id, Power::ZERO};
}

void Receiver::processConnectionLost(const ConnectionLost& update)
{
  int peer_id = getPeerID(update.reader_id, update.tag_id);
  auto peer_iter = peers.find(peer_id);
  if (peer_iter != peers.end())
  {
    peers.erase(peer_iter);
    auto rxop_iter = rxops_by_peer.find(peer_id);
    if (rxop_iter != rxops_by_peer.end())
    {
      auto rxop = rxop_iter->second;
      rxops_by_peer.erase(rxop_iter);
      rxops_by_timer.erase(rxop->timer);
      rxops.erase(rxop);
      cancelAndDelete(rxop->timer);
      delete rxop->frame;
      delete rxop;
    }
  }
  else
  {
    ASSERT2(rxops_by_peer.count(peer_id) == 0,
            "RXOPs from unknown devices disallowed");
  }
}

void Receiver::setState(State state)
{
  this->state = state;
}

void Receiver::updatePeerPower(int peer_id, const Power& power)
{
  // Update RXOP from that peer
  auto rxop_iter = rxops_by_peer.find(peer_id);
  if (rxop_iter != rxops_by_peer.end())
  {
    auto rxop = rxop_iter->second;
    auto frame_power = power.amplify(rxop->frame->getModulationLoss());

    // Update RXOP power
    if (rxop->min_power > frame_power)
      rxop->min_power = frame_power;
    if (rxop->max_power < frame_power)
      rxop->max_power = frame_power;
  }

  // Update the peer power
  auto peer_iter = peers.find(peer_id);
  if (peer_iter != peers.end())
    peer_iter->second.power = power;
  else
    ASSERT2(rxop_iter == rxops_by_peer.end(),
            "RXOPs from unknown devices disallowed");
}

int Receiver::getPeerID(int reader_id, int tag_id)
{
  return device_id == reader_id ? tag_id : reader_id;
}

}
