#ifndef RFIDSIMPP_COMMON_UNITS_H_
#define RFIDSIMPP_COMMON_UNITS_H_

#ifndef POWER_WATT_ZERO_TOL
#define POWER_WATT_ZERO_TOL 1e-24
#endif

#include <omnetpp.h>

namespace rfidsim {

class Decibel : public omnetpp::cObject {
 public:
  double value = 0.0;

  static Decibel from_linear_scale(double value, double reference = 1.0);

  static Decibel from_log_scale(double value) {
    return Decibel(value);
  }

  Decibel(double db) : value(db) {}
  Decibel(const Decibel& rside) : value(rside.value) {}
  virtual ~Decibel() {}

  Decibel& operator=(const Decibel& rside) {
    value = rside.value;
    return *this;
  }

  Decibel operator+(const Decibel& rside) const {
    return Decibel(value + rside.value);
  }

  Decibel operator-(const Decibel& rside) const {
    return Decibel(value - rside.value);
  }

  double linear_value(double reference = 1.0) const {
    return pow(10.0, value / 10.0) * reference;
  }

  virtual Decibel* dup() const {
    return new Decibel(*this);
  }

  std::string to_string() const {
    return info();
  }

  virtual std::string info() const {
    static char buf[16];
    snprintf(buf, sizeof(buf), "%.4fdB", value);
    return buf;
  }
};


class Power : public omnetpp::cObject {
public:
  enum Unit { DBM, WATT };

  static const char *str(Unit unit);
  static double convert(double value, Unit from, Unit to);

  Power() : value_(0.0), unit_(WATT) {}
  Power(double value, Unit unit) { set(value, unit); }
  Power(const Power& other) : value_(other.value_), unit_(other.unit_) {}
  virtual ~Power() {}

  Power& operator=(const Power& rside) {
    return set(rside.value(), rside.unit());
  }

  virtual Power *dup() const {
    return new Power(*this);
  }

  double value() const { return value_; }
  Unit unit() const { return unit_; }

  double get_as(Unit unit) const {
    return convert(value_, unit_, unit);
  }

  double watt() const {
    return get_as(WATT);
  }

  double dbm() const {
    return get_as(DBM);
  }

  void rebase(Unit unit) {
    value_ = get_as(unit);
  }

  /**
   * Set power and storage unit to the given values. If storage type must
   * be unchanged, use assign() method instead.
   */
  Power& set(double value, Unit unit) {
    assert_correct_value(value, unit);
    value_ = value;
    unit_ = unit;
    return *this;
  }

  /**
   * Set power to the given value with the given unit, but keeps the storage
   * unit type. If storage type must be also updated, use set() method instead.
   */
  Power& assign(double value, Unit unit) {
    assert_correct_value(value, unit);
    value_ = convert(value, unit, unit_);
    return *this;
  }

  /* Adding and subtracting power (in Watts) */

  Power add(const Power& rside) const {
    return Power(watt() + rside.watt(), WATT);
  }

  Power sub(const Power& rside) const {
    return Power(watt() - rside.watt(), WATT);
  }

  Decibel diff(const Power& rside) const {
    return Decibel(dbm() - rside.dbm());
  }

  /* Multiplying power in linear scale */

  Power mul(double k) const {
    return Power(watt() * k, WATT);
  }

  Power div(double k) const {
    return Power(watt() / k, WATT);
  }

  /* Attenuating and amplifying - adding and subtracting in log scale (dB) */

  Power amplify(const Decibel& db) const {
    return Power(dbm() + db.value, DBM);
  }

  std::string to_string(Unit unit, bool adjust_scale = true) const;

  virtual std::string info() const {
    return to_string(unit_);
  }

private:
  double value_;
  Unit unit_;

  static void assert_correct_value(double value, Unit unit) {
    if (unit == WATT && value < 0) {
      throw omnetpp::cRuntimeError("negative power value in Watts prohibited");
    }
  }
};

}

#endif
