#ifndef RFIDSIMPP_WORLD_MOBILITY_MOBILITY_UNIT_BASE_H_
#define RFIDSIMPP_WORLD_MOBILITY_MOBILITY_UNIT_BASE_H_

#include <omnetpp.h>
#include <common/units.h>
#include <common/optional.hpp>
#include <common/geometry.h>

namespace rfidsim {

class MobilityController;

class MobilityUnit : public omnetpp::cSimpleModule, public omnetpp::cListener {
 public:
  static omnetpp::simsignal_t POSITION_UPDATED_SIGNAL_ID;

  enum BoundType {
    UNBOUND, SOFT_BOUND, HARD_BOUND
  };

  static BoundType getBoundTypeFromPar(const omnetpp::cPar& par);

  virtual ~MobilityUnit();

  const Vector3& getRelativePosition() const { return relative_pos; }
  const Vector3& getAbsolutePosition() const { return absolute_pos; }
  const CoordSystem& getRelativeCS() const { return relative_cs; }
  const CoordSystem& getAbsoluteCS() const { return absolute_cs; }

  const nonstd::optional<omnetpp::simtime_t>& getLastUpdateTime() {
    return last_update_time;
  }

  bool isBound() const { return bound_type != UNBOUND; }
  bool isUnbound() const { return bound_type == UNBOUND; }
  bool isForwardingUpdates() const { return forward_updates; }
  bool isDeviceMU() const { return device_mobility_unit; }

 protected:
  virtual int numInitStages() const;
  virtual void initialize(int stage);

  virtual void receiveSignal(
          omnetpp::cComponent *source, omnetpp::simsignal_t signal_id,
          omnetpp::cObject *obj, omnetpp::cObject *details);

  virtual void processPositionUpdate();

  virtual void getNextPosition(Vector3 *next_pos, CoordSystem *next_cs) = 0;

 private:
  MobilityController *controller = nullptr;
  MobilityUnit *master_unit = nullptr;
  BoundType bound_type = UNBOUND;
  bool forward_updates = true;
  bool device_mobility_unit = false;
  int device_id = -1;

  nonstd::optional<omnetpp::simtime_t> last_update_time;
  Vector3 relative_pos;
  Vector3 absolute_pos;
  CoordSystem relative_cs;
  CoordSystem absolute_cs;

  std::map<omnetpp::cModule*, omnetpp::simsignal_t> subscriptions;

  void updatePosition();

  void emitPositionUpdated(const Vector3& next_pos, const Vector3& prev_pos,
                           const CoordSystem& prev_cs,
                           const CoordSystem& next_cs);

  void emitPositionUpdated(const Vector3& pos, const CoordSystem& cs);

  void setPosition(const Vector3& relative_pos);
  void setCS(const CoordSystem& relative_cs);
};

}



#endif /* MOBILITY_MOBILITY_UNITS_H_ */
