#ifndef RFIDSIMPP_WORLD_MOBILITY_MOBILITY_UNITS_H_
#define RFIDSIMPP_WORLD_MOBILITY_MOBILITY_UNITS_H_

#include <world/mobility/mobility-unit-base.h>

namespace rfidsim {

class StationaryMobilityUnit : public MobilityUnit {
 public:
  virtual ~StationaryMobilityUnit() {}

  virtual void getNextPosition(Vector3 *next_pos, CoordSystem *next_cs) {
    if (next_pos) *next_pos = getAbsolutePosition();
    if (next_cs) *next_cs = getAbsoluteCS();
  }
};

}

#endif
