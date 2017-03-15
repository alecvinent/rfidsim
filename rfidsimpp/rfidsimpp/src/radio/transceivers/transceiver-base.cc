#include <radio/transceivers/transceiver-base.h>
#include <radio/antenna-base.h>
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

const char *Transmitter::strState(State state)
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

  subscriptions.clear();
  auto device = findEnclosingRFIDDevice(this);
  auto sys = getSystemModule();
  subscriptions[Antenna::RECEIVED_FIELD_OFF_SIGNAL_ID] = device;
  subscriptions[Antenna::RECEIVED_FIELD_ON_SIGNAL_ID] = device;
  subscriptions[PowerUnit::POWER_OFF_SIGNAL_ID] = device;
  subscriptions[PowerUnit::POWER_ON_SIGNAL_ID] = device;
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

void Transmitter::processTimeout(cMessage *timeout)
{
  //TODO
}

void Transmitter::processSendDataReq(SendDataReq *request)
{
  //TODO
}

void Transmitter::processReceivedFieldOff(const ReceivedFieldOff& update)
{
  //TODO
}

void Transmitter::processReceivedFieldOn(const ReceivedFieldOn& updated)
{
  //TODO
}

void Transmitter::processPowerOn(const PowerOn& update)
{
  //TODO
}

void Transmitter::processPowerOff(const PowerOff& update)
{
  //TODO
}

void Transmitter::processConnectionCreated(const ConnectionCreated& update)
{
  //TODO
}

void Transmitter::processConnectionLost(const ConnectionLost& update)
{
  //TODO
}



//
//===========================================================================
// Receiver
//===========================================================================
//
Receiver::~Receiver()
{
  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->unsubscribe(i->first, this);
}

ChannelModel *Receiver::getChannelModel() const
{
  return channel;
}

void Receiver::initialize()
{
  device_id = static_cast<int>(par("deviceID").longValue());
  state = OFF;
  channel = findChildByType<ChannelModel>(getParentModule());

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
      processReceivedFieldUpdate(*signal_);
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

void Receiver::processTimeout(cMessage *timeout)
{
  //TODO
}

void Receiver::processAirFrame(AirFrame *air_frame)
{
  //TODO
}

void Receiver::processTxStart(const TxStart& update)
{
  //TODO
}

void Receiver::processTxFinish(const TxFinish& update)
{
  //TODO
}

void Receiver::processReceivedFieldOff(const ReceivedFieldOff& update)
{
  //TODO
}

void Receiver::processReceivedFieldOn(const ReceivedFieldOn& updated)
{
  //TODO
}

void Receiver::processReceivedFieldUpdate(const ReceivedFieldUpdated& update)
{
  //TODO
}

void Receiver::processPowerOn(const PowerOn& update)
{
  //TODO
}

void Receiver::processPowerOff(const PowerOff& update)
{
  //TODO
}

void Receiver::processConnectionCreated(const ConnectionCreated& update)
{
  //TODO
}

void Receiver::processConnectionLost(const ConnectionLost& update)
{
  //TODO
}

}
