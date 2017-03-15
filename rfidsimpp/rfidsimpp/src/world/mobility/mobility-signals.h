#ifndef RFIDSIMPP_WORLD_MOBILITY_MOBILITY_SIGNALS_H_
#define RFIDSIMPP_WORLD_MOBILITY_MOBILITY_SIGNALS_H_

#include <omnetpp.h>
#include <common/geometry.h>
#include <common/optional.hpp>

namespace rfidsim {


class PositionUpdated : public omnetpp::cObject, omnetpp::noncopyable {
 public:
  static const char *NAME;

  nonstd::optional<Vector3> prev_position;
  nonstd::optional<Vector3> next_position;
  nonstd::optional<CoordSystem> prev_cs;
  nonstd::optional<CoordSystem> next_cs;
  nonstd::optional<omnetpp::simtime_t> prev_update_time;

  virtual ~PositionUpdated() {}
};



class DevicePositionUpdated : public PositionUpdated {
 public:
  int device_id;

  virtual ~DevicePositionUpdated() {}
};



class UpdatePosition : public omnetpp::cObject, omnetpp::noncopyable {
 public:
  static const char *NAME;

  virtual ~UpdatePosition() {}
};



}

#endif /* MOBILITY_MOBILITY_SIGNALS_H_ */
