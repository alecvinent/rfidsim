#include <power/power-unit-base.h>
#include <power/power-unit-signals.h>

using namespace omnetpp;

namespace rfidsim {

simsignal_t PowerUnit::POWER_ON_SIGNAL_ID = registerSignal(PowerOn::NAME);
simsignal_t PowerUnit::POWER_OFF_SIGNAL_ID = registerSignal(PowerOff::NAME);
simsignal_t PowerUnit::POWER_UPDATED_SIGNAL_ID =
        registerSignal(PowerUpdated::NAME);

void PowerUnit::initialize() {
  device_id = static_cast<int>(par("deviceID").longValue());
}

void PowerUnit::emitPowerOn(const Power& power) {
  PowerOn power_on(device_id, power);
  emit(POWER_ON_SIGNAL_ID, &power_on);
}

void PowerUnit::emitPowerOff() {
  PowerOff power_off(device_id);
  emit(POWER_OFF_SIGNAL_ID, &power_off);
}

void PowerUnit::emitPowerUpdate(const Power& power) {
  PowerUpdated power_updated(device_id, power);
  emit(POWER_UPDATED_SIGNAL_ID, &power_updated);
}

}
