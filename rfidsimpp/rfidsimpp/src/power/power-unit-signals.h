#ifndef RFIDSIMPP_POWER_POWER_UNIT_SIGNALS_H_
#define RFIDSIMPP_POWER_POWER_UNIT_SIGNALS_H_

#include <omnetpp.h>
#include <common/units.h>

namespace rfidsim {

class PowerOff : public omnetpp::cObject, omnetpp::noncopyable {
 public:
  static const char *NAME;
  int device_id = -1;

  PowerOff() {}
  PowerOff(int device_id) : device_id(device_id) {}

  virtual std::string info() const;
};

class PowerOn : public omnetpp::cObject, omnetpp::noncopyable {
 public:
  static const char *NAME;
  int device_id = -1;
  Power power = Power::ZERO;

  PowerOn() {}
  PowerOn(int device_id, const Power& power)
    : device_id(device_id), power(power) {}

  virtual std::string info() const;
};


class PowerUpdated : public omnetpp::cObject, omnetpp::noncopyable {
 public:
  static const char *NAME;
  int device_id = -1;
  Power power = Power::ZERO;

  PowerUpdated() {}
  PowerUpdated(int device_id, const Power& power)
    : device_id(device_id), power(power) {}

  virtual std::string info() const;
};

}

#endif
