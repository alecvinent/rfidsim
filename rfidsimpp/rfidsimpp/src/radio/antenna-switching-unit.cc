#include <radio/antenna-switching-unit.h>
#include <radio/antenna-base.h>
#include <common/rfidsim-defs.h>
#include <common/functions.h>
#include <common/module-access.h>
#include <power/power-unit-base.h>

using namespace omnetpp;

namespace rfidsim {

Define_Module(AntennaSwitchingUnit);

simsignal_t AntennaSwitchingUnit::SWITCH_ANTENNA_SIGNAL_ID =
        registerSignal(SwitchAntenna::NAME);

AntennaSwitchingUnit::~AntennaSwitchingUnit()
{
  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->unsubscribe(i->first, this);
  cancelAndDelete(timer);
}

int AntennaSwitchingUnit::numInitStages() const
{
  return maximum(INIT_STAGE_BASE, INIT_STAGE_ANTENNA_SWITCH_EMIT) + 1;
}

void AntennaSwitchingUnit::initialize(int stage)
{
  if (stage == INIT_STAGE_BASE)
  {
    switch_only_when_powered = par("switchOnlyWhenPowered").boolValue();
    interval = SimTime(par("interval"));
    device_id = static_cast<int>(par("deviceID").longValue());

    is_powered = false;

    auto device = findEnclosingRFIDDevice(this);
    subscriptions.clear();
    subscriptions[PowerUnit::POWER_OFF_SIGNAL_ID] = device;
    subscriptions[PowerUnit::POWER_ON_SIGNAL_ID] = device;
    for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
      i->second->subscribe(i->first, this);

    char timer_name[32];
    snprintf(timer_name, sizeof(timer_name), "ASU-%d-Timer", device_id);
    timer = new cMessage("ASU-%d-Timer");

    antennas.clear();
    num_antennas = 0;
    current_index = 0;
  }
  else if (stage == INIT_STAGE_ANTENNA_SWITCH_EMIT)
  {
    // Discover antennas - doing this here since setIndex() may be performed
    // by antennas in the earlier stages
    num_antennas = gateSize("antennaOut");
    antennas.resize(num_antennas, nullptr);
    for (int i = 0; i < num_antennas; ++i)
    {
      Antenna *antenna = dynamic_cast<Antenna*>(gate("antennaOut", i));
      if (!antenna)
      {
        throw cRuntimeError("antenna not found at antennaOut[%d]", i);
      }
      antenna->setIndex(i);
      antennas[i] = antenna;
    }

    // If signal can already be emitter, do it below! Otherwise the emit will
    // be taken after PowerOn signal received.
    switchAntenna(0);
  }
}

void AntennaSwitchingUnit::receiveSignal(cComponent *source,
                                         simsignal_t signal_id, cObject *obj,
                                         cObject *details)
{
  if (signal_id == PowerUnit::POWER_ON_SIGNAL_ID)
  {
    PowerOn *signal_ = static_cast<PowerOn*>(obj);
    if (signal_->device_id == device_id)
      processPowerOn(*signal_);
  }
  else if (signal_id == PowerUnit::POWER_OFF_SIGNAL_ID)
  {
    PowerOff *signal_ = static_cast<PowerOff*>(obj);
    if (signal_->device_id == device_id)
      processPowerOff(*signal_);
  }
}

void AntennaSwitchingUnit::handleMessage(cMessage *msg)
{
  if (msg == timer)
  {
    processTimer();
  }
  else
  {
    throw cRuntimeError("unexpected message '%s'", msg->getFullName());
  }
}

void AntennaSwitchingUnit::processTimer()
{
  switchAntenna();
}

void AntennaSwitchingUnit::processPowerOn(const PowerOn& upd)
{
  is_powered = true;
  if (switch_only_when_powered)
    switchAntenna();
}

void AntennaSwitchingUnit::processPowerOff(const PowerOff& upd)
{
  if (switch_only_when_powered && timer->isScheduled())
    cancelEvent(timer);
  is_powered = false;
}

void AntennaSwitchingUnit::switchAntenna(int next_index)
{
  if (num_antennas > 0 && (is_powered || !switch_only_when_powered))
  {
    current_index =
            next_index < 0 ? (current_index + 1) % num_antennas : next_index;
    SwitchAntenna signal_(device_id, current_index);
    emit(SWITCH_ANTENNA_SIGNAL_ID, &signal_);
    scheduleAt(simTime() + interval, timer);
  }
}

}
