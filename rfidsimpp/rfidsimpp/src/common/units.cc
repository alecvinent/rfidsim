#include <common/units.h>
#include <limits>

using namespace omnetpp;

namespace rfidsim {

Decibel Decibel::from_linear_scale(double value, double reference) {
  double arg = value / reference;
  if (arg < 0) {
    static char err_buf[128];
    snprintf(err_buf, sizeof(err_buf),
             "negative linear amplification with value=%f, reference=%f",
             value, reference);
    throw cRuntimeError(err_buf);
  }
  return Decibel(arg);
}

const Decibel Decibel::ZERO = Decibel(0.0);


const Power Power::ZERO = Power();

const char *Power::str(Power::Unit unit) {
  if (unit == WATT)
    return "W";
  else if (unit == DBM) {
    return "dBm";
  } else {
    static char buf[64];
    snprintf(buf, sizeof(buf), "unexpected unit %d", unit);
    throw cRuntimeError(buf);
  }
}

double Power::convert(double value, Unit from, Unit to) {
  if (to == from) {
    return value;
  } else if (to == DBM && from == WATT) {
    if (value < POWER_WATT_ZERO_TOL) {
      return -std::numeric_limits<double>::infinity();
    } else {
      return 30.0 + 10 * log10(value);
    }
  } else if (to == WATT && from == DBM) {
    return pow(10.0, value/10 - 3);
  } else {
    static char buf[128];
    snprintf(buf, sizeof(buf), "unexpected units from=%d or to=%d", from, to);
    throw cRuntimeError(buf);
  }
}

int Power::compare(const Power& other) const {
  auto lside = value_;
  auto rside = other.getAs(unit_);

  if (lside < rside)
    return -1;
  else if (lside > rside)
    return 1;
  else
    return 0;
}

std::string Power::toString(Unit unit, bool adjust_scale) const {
  static char buf[128];
  double value;
  const char *unit_string;
  if (unit == DBM) {
    value = dbm();
    unit_string = "dBm";
  } else if (unit == WATT) {
    if (adjust_scale) {
      double w = watt();
      if (w < 1e-9) {
        value = w * 1e12;
        unit_string = "pW";
      } else if (w < 1e-6) {
        value = w * 1e9;
        unit_string = "nW";
      } else if (w < 1e-3) {
        value = w * 1e6;
        unit_string = "uW";
      } else if (w < 1.0) {
        value = w * 1e3;
        unit_string = "mW";
      } else {
        value = w;
        unit_string = "W";
      }
    } else {
      value = watt();
      unit_string = "W";
    }
  } else {
    snprintf(buf, sizeof(buf), "unexpected unit type %d",
             static_cast<int>(unit));
    throw cRuntimeError(buf);
  }
  snprintf(buf, sizeof(buf), "%f%s", value, unit_string);
  return buf;
}


}
