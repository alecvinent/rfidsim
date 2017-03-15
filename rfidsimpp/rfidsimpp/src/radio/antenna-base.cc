#include <radio/antenna-base.h>
#include <radio/antenna-signals.h>
#include <radio/antenna-switching-unit.h>
#include <power/power-unit-signals.h>
#include <power/power-unit-base.h>
#include <common/module-access.h>
#include <common/rfidsim-defs.h>
#include <common/units.h>
#include <common/functions.h>
#include <medium/medium.h>

using namespace omnetpp;

namespace rfidsim {

//
//===========================================================================
// Antenna
//===========================================================================
//
simsignal_t Antenna::RECEIVED_FIELD_OFF_SIGNAL_ID =
        registerSignal(ReceivedFieldOff::NAME);

simsignal_t Antenna::RECEIVED_FIELD_ON_SIGNAL_ID =
        registerSignal(ReceivedFieldOn::NAME);

simsignal_t Antenna::RECEIVED_FIELD_UPDATED_SIGNAL_ID =
        registerSignal(ReceivedFieldUpdated::NAME);

simsignal_t Antenna::RADIATED_FIELD_OFF_SIGNAL_ID =
        registerSignal(RadiatedFieldOff::NAME);

simsignal_t Antenna::RADIATED_FIELD_ON_SIGNAL_ID =
        registerSignal(RadiatedFieldOn::NAME);

simsignal_t Antenna::RADIATED_FIELD_UPDATED_SIGNAL_ID =
        registerSignal(RadiatedFieldUpdated::NAME);

simsignal_t Antenna::ANTENNA_ACTIVATED_SIGNAL_ID =
        registerSignal(AntennaActivated::NAME);

Antenna::~Antenna()
{
  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->unsubscribe(i->first, this);
}

std::string Antenna::getAntennaName() const
{
  static char buf[256];
  snprintf(buf, sizeof(buf), "%s-%d-%d", getAntennaTypeName(), getDeviceID(),
           getIndex());
  return buf;
}

Decibel Antenna::getRxLoss(const Vector3& direction) const
{
  auto d = -1 * direction;
  auto cs = getAbsoluteCS();
  auto cos_x = d.getCosWith(cs.getEx());
  auto cos_y = d.getCosWith(cs.getEy());
  return getLoss(cos_x, cos_y);
}

Decibel Antenna::getTxLoss(const Vector3& direction) const
{
  auto cs = getAbsoluteCS();
  auto cos_x = direction.getCosWith(cs.getEx());
  auto cos_y = direction.getCosWith(cs.getEy());
  return getLoss(cos_x, cos_y);
}

int Antenna::numInitStages() const
{
  return maximum(maximum(INIT_STAGE_BASE, INIT_STAGE_ANTENNA_ACTIVATION) + 1,
                 MobilityUnit::numInitStages());
}

void Antenna::initialize(int stage)
{
  MobilityUnit::initialize(stage);
  if (stage == INIT_STAGE_BASE)
  {
    is_active = par("activated").boolValue();
    cable_loss = Decibel(par("cableLoss").doubleValue());
    gain = Decibel(par("gain").doubleValue());

    is_powered = false;
    power = Power::ZERO;

    medium = findChildByType<Medium>(getSystemModule());

    auto sys = getSystemModule();
    auto device = findEnclosingRFIDDevice(this);
    subscriptions.clear();
    subscriptions[RADIATED_FIELD_OFF_SIGNAL_ID] = sys;
    subscriptions[RADIATED_FIELD_ON_SIGNAL_ID] = sys;
    subscriptions[RADIATED_FIELD_UPDATED_SIGNAL_ID] = sys;
    subscriptions[PowerUnit::POWER_OFF_SIGNAL_ID] = device;
    subscriptions[PowerUnit::POWER_ON_SIGNAL_ID] = device;
    subscriptions[PowerUnit::POWER_UPDATED_SIGNAL_ID] = device;
    subscriptions[AntennaSwitchingUnit::SWITCH_ANTENNA_SIGNAL_ID] = device;

    for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
      i->second->subscribe(i->first, this);
  }
  else if (stage == INIT_STAGE_ANTENNA_ACTIVATION)
  {
    if (is_active)
    {
      AntennaActivated antenna_activated(getDeviceID(), this);
      emit(ANTENNA_ACTIVATED_SIGNAL_ID, &antenna_activated);
    }
  }
}

void Antenna::receiveSignal(cComponent *source, simsignal_t signal_id,
                            cObject *obj, cObject *details)
{
  if (signal_id == RADIATED_FIELD_OFF_SIGNAL_ID)
  {
    RadiatedFieldOff *upd = static_cast<RadiatedFieldOff*>(obj);
    if (upd->device_id != device_id)
      processRadiatedFieldOff(*upd);
  }
  else if (signal_id == RADIATED_FIELD_ON_SIGNAL_ID)
  {
    RadiatedFieldOn *upd = static_cast<RadiatedFieldOn*>(obj);
    if (upd->device_id != device_id)
      processRadiatedFieldOn(*upd);
  }
  else if (signal_id == RADIATED_FIELD_UPDATED_SIGNAL_ID)
  {
    RadiatedFieldUpdated *upd = static_cast<RadiatedFieldUpdated*>(obj);
    if (upd->device_id != device_id)
      processRadiatedFieldUpdated(*upd);
  }
  else if (signal_id == PowerUnit::POWER_OFF_SIGNAL_ID)
  {
    PowerOff *upd = static_cast<PowerOff*>(obj);
    if (upd->device_id == device_id)
      processPowerOff(*upd);
  }
  else if (signal_id == PowerUnit::POWER_ON_SIGNAL_ID)
  {
    PowerOn *upd = static_cast<PowerOn*>(obj);
    if (upd->device_id == device_id)
      processPowerOn(*upd);
  }
  else if (signal_id == PowerUnit::POWER_UPDATED_SIGNAL_ID)
  {
    PowerUpdated *upd = static_cast<PowerUpdated*>(obj);
    if (upd->device_id == device_id)
      processPowerUpdated(*upd);
  }
  else if (signal_id == AntennaSwitchingUnit::SWITCH_ANTENNA_SIGNAL_ID)
  {
    SwitchAntenna *upd = static_cast<SwitchAntenna*>(obj);
    if (upd->device_id == device_id)
      processSwitchAntenna(*upd);
  }
  else
  {
    MobilityUnit::receiveSignal(source, signal_id, obj, details);
  }
}

void Antenna::processPositionUpdate()
{
  if (!is_active || !is_powered) return;
  RadiatedFieldUpdated signal_(device_id, this, power);
  emit(RADIATED_FIELD_UPDATED_SIGNAL_ID, &signal_);
}

void Antenna::processRadiatedFieldOff(const RadiatedFieldOff& upd)
{
  if (!is_active) return;
  ReceivedFieldOff signal_(device_id, upd.device_id);
  emit(RECEIVED_FIELD_OFF_SIGNAL_ID, &signal_);
}

void Antenna::processRadiatedFieldOn(const RadiatedFieldOn& upd)
{
  if (!is_active) return;
  auto path_loss = medium->getPathLoss(upd.peer_antenna, this);
  auto full_loss = path_loss + cable_loss;
  auto received_power = upd.radiated_power.amplify(full_loss);
  ReceivedFieldOn signal_(upd.device_id, upd.peer_antenna, received_power);
  emit(RECEIVED_FIELD_ON_SIGNAL_ID, &signal_);
}

void Antenna::processRadiatedFieldUpdated(const RadiatedFieldUpdated& upd)
{
  if (!is_active) return;
  auto path_loss = medium->getPathLoss(upd.peer_antenna, this);
  auto full_loss = path_loss + cable_loss;
  auto received_power = upd.radiated_power.amplify(full_loss);
  ReceivedFieldUpdated signal_(upd.device_id, upd.peer_antenna, received_power);
  emit(RECEIVED_FIELD_UPDATED_SIGNAL_ID, &signal_);
}

void Antenna::processPowerOff(const PowerOff& upd)
{
  power = Power::ZERO;
  is_powered = false;
  if (is_active)
  {
    RadiatedFieldOff signal_(device_id);
    emit(RADIATED_FIELD_OFF_SIGNAL_ID, &signal_);
  }
}

void Antenna::processPowerOn(const PowerOn& upd)
{
  power = upd.power.amplify(cable_loss);
  is_powered = true;
  if (is_active)
  {
    RadiatedFieldOn signal_(device_id, this, power);
    emit(RADIATED_FIELD_ON_SIGNAL_ID, &signal_);
  }
}

void Antenna::processPowerUpdated(const PowerUpdated& upd)
{
  power = upd.power.amplify(cable_loss);
  if (is_active)
  {
    RadiatedFieldUpdated signal_(device_id, this, power);
    emit(RADIATED_FIELD_UPDATED_SIGNAL_ID, &signal_);
  }
}

void Antenna::processSwitchAntenna(const SwitchAntenna& upd)
{
  if (is_active && upd.index != this->index)
  {
    is_active = false;
  }
  else if (!is_active && upd.index == this->index)
  {
    is_active = true;
    AntennaActivated signal_(device_id, this);
    emit(ANTENNA_ACTIVATED_SIGNAL_ID, &signal_);
  }
}

}
