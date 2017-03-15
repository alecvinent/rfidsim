#include <common/module-access.h>
#include <common/functions.h>
#include <common/rfidsim-defs.h>
#include <world/mobility/mobility-signals.h>
#include <world/mobility/mobility-unit-base.h>
#include <world/mobility/mobility-controller.h>

using namespace omnetpp;

namespace rfidsim {

simsignal_t MobilityUnit::POSITION_UPDATED_SIGNAL_ID =
        registerSignal(PositionUpdated::NAME);

MobilityUnit::BoundType MobilityUnit::getBoundTypeFromPar(const cPar& par)
{
  auto s = par.stdstringValue();
  if (s == "" || s == "unbound") {
    return UNBOUND;
  } else if (s == "hard" || s == "hardbound") {
    return HARD_BOUND;
  } else if (s == "soft" || s == "softbound") {
    return SOFT_BOUND;
  } else {
    throw cRuntimeError("unrecognized bound type '%s'", s.c_str());
  }
}

MobilityUnit::~MobilityUnit()
{
  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i) {
    i->first->unsubscribe(i->second, this);
  }
}

int MobilityUnit::numInitStages() const
{
  return maximum(INIT_STAGE_BASE, INIT_STAGE_MOBILITY_UNITS_EMIT) + 1;
}

void MobilityUnit::initialize(int stage)
{
  if (stage == INIT_STAGE_BASE)
  {
    // Searching for the controller and the master MU.
    controller = findModuleFromPar<MobilityController>(par("controller"), this);
    master_unit = findModuleFromPar<MobilityUnit>(par("masterUnit"), this);

    // Unit can not be governed by both Master MU and Controller. Check this.
    if (master_unit && controller) {
      throw cRuntimeError("ambiguous mobility unity control: both master unit "
              "'%s' and controller '%s' specified",
              par("controller").stringValue(), par("masterUnit").stringValue());
    }
    forward_updates = par("forwardUpdates").boolValue();
    device_mobility_unit = par("isDeviceMobilityUnit").boolValue();

    // Bound type is meaningful if and only if the unit is governed by another
    // (master) mobility unit. If so, take it from the parameter
    bound_type =
            master_unit ? getBoundTypeFromPar(par("masterBoundType")) : UNBOUND;

    relative_pos = Vector3(par("x0"), par("y0"), par("z0"));
    relative_cs = CoordSystem(Vector3(par("fwdX"), par("fwdY"), par("fwdZ")),
                              Vector3(par("upX"), par("upY"), par("upZ")));
    setPosition(relative_pos);
    setCS(relative_cs);
  }
  else if (stage == INIT_STAGE_MOBILITY_UNITS_EMIT)
  {
    // Since we don't want to duplicate PositionUpdates, modules will subscribe
    // to their controllers/masters only after emitting PositionUpdated
    // messages without previous position and coordination system set.
    emitPositionUpdated(absolute_pos, absolute_cs);

    if (controller) {
      auto parent = getClosestCommonParent(this, controller);
      parent->subscribe(MobilityController::UPDATE_POSITION_SIGNAL_ID, this);
      subscriptions[parent] = MobilityController::UPDATE_POSITION_SIGNAL_ID;
    }
    if (master_unit) {
      auto parent = getClosestCommonParent(this, master_unit);
      parent->subscribe(MobilityUnit::POSITION_UPDATED_SIGNAL_ID, this);
      subscriptions[parent] = MobilityUnit::POSITION_UPDATED_SIGNAL_ID;
    }
  }
}

void MobilityUnit::receiveSignal(cComponent *source, simsignal_t signal_id,
                                 cObject *obj, cObject *details)
{
  if (signal_id == MobilityController::UPDATE_POSITION_SIGNAL_ID)
  {
    if (controller && dynamic_cast<MobilityController*>(source) == controller)
    {
      ASSERT(isUnbound());
      updatePosition();
    }
  }
  else if (signal_id == MobilityUnit::POSITION_UPDATED_SIGNAL_ID)
  {
    if (master_unit && dynamic_cast<MobilityUnit*>(source) == master_unit)
    {
      ASSERT(isUnbound());
      updatePosition();
    }
  }
}

void MobilityUnit::processPositionUpdate()
{
  // nothing is done here, but maybe in derived classes?
  // See antennas as an example.
}

void MobilityUnit::updatePosition()
{
  Vector3 prev_pos = absolute_pos;
  CoordSystem prev_cs = absolute_cs;

  getNextPosition(&relative_pos, &relative_cs);

  setPosition(relative_pos);
  setCS(relative_cs);

  emitPositionUpdated(absolute_pos, prev_pos, absolute_cs, prev_cs);
  processPositionUpdate();
}


void MobilityUnit::emitPositionUpdated(
        const Vector3& next_pos, const Vector3& prev_pos,
        const CoordSystem& prev_cs, const CoordSystem& next_cs)
{
  PositionUpdated normal_upd;
  DevicePositionUpdated device_upd;
  PositionUpdated *upd_ptr = nullptr;

  if (isDeviceMU()) {
    upd_ptr = &device_upd;
    device_upd.device_id = device_id;
  } else {
    upd_ptr = &normal_upd;
  }

  upd_ptr->next_position = next_pos;
  upd_ptr->next_cs = next_cs;
  upd_ptr->prev_position = prev_pos;
  upd_ptr->prev_cs = prev_cs;
  upd_ptr->prev_update_time = last_update_time;

  emit(POSITION_UPDATED_SIGNAL_ID, upd_ptr);
}

void MobilityUnit::emitPositionUpdated(const Vector3& pos,
                                       const CoordSystem& cs)
{
  PositionUpdated normal_upd;
  DevicePositionUpdated device_upd;
  PositionUpdated *upd_ptr = nullptr;

  if (isDeviceMU()) {
    upd_ptr = &device_upd;
    device_upd.device_id = device_id;
  } else {
    upd_ptr = &normal_upd;
  }

  upd_ptr->next_position = pos;
  upd_ptr->next_cs = cs;

  upd_ptr->prev_position = nonstd::nullopt;
  upd_ptr->prev_cs = nonstd::nullopt;
  upd_ptr->prev_update_time = nonstd::nullopt;

  emit(POSITION_UPDATED_SIGNAL_ID, upd_ptr);
}

void MobilityUnit::setPosition(const Vector3& relative_pos)
{
  this->relative_pos = relative_pos;
  if (bound_type == UNBOUND) {
    this->absolute_pos = relative_pos;
  } else {
    this->absolute_pos = master_unit->getAbsoluteCS().relVecToAbs(relative_pos);
  }
}

void MobilityUnit::setCS(const CoordSystem& relative_cs)
{
  this->relative_cs = relative_cs;
  if (bound_type == HARD_BOUND) {
    this->absolute_cs = master_unit->getAbsoluteCS().relCsToAbs(relative_cs);
  } else {
    this->absolute_cs = relative_cs;
  }
}

}
