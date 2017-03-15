#ifndef RFIDSIMPP_COMMON_GEOMETRY_H_
#define RFIDSIMPP_COMMON_GEOMETRY_H_

#include <omnetpp.h>

namespace rfidsim {

class Vector3 : public omnetpp::cObject {
 public:
  double x;
  double y;
  double z;

  static const Vector3 ZERO;
  static const Vector3 EX;
  static const Vector3 EY;
  static const Vector3 EZ;

  Vector3()
   : Vector3(0.0, 0.0, 0.0) {}

  Vector3(double x, double y, double z)
    : x(x), y(y), z(z) {}

  Vector3(const Vector3& other)
    : x(other.x), y(other.y), z(other.z) {}

  Vector3(const omnetpp::cPar& x, const omnetpp::cPar& y,
          const omnetpp::cPar& z)
    : x(x.doubleValue()), y(y.doubleValue()), z(z.doubleValue()) {}

  virtual ~Vector3() {}

  Vector3& set(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
    return *this;
  }

  Vector3& operator=(const Vector3& rside) {
    return set(rside.x, rside.y, rside.z);
  }

  Vector3 operator+(const Vector3& rside) const {
    return Vector3(x + rside.x, y + rside.y, z + rside.z);
  }

  Vector3 operator-(const Vector3& rside) const {
    return Vector3(x - rside.x, y - rside.y, z - rside.z);
  }

  Vector3 operator*(double k) const {
    return Vector3(x * k, y * k, z * k);
  }

  Vector3 operator/(double k) const {
    return Vector3(x / k, y / k, z / k);
  }

  Vector3& operator+=(const Vector3& rside) {
    return set(x + rside.x, y + rside.y, z + rside.z);
  }

  Vector3& operator-=(const Vector3& rside) {
    return set(x - rside.x, y - rside.y, z - rside.z);
  }

  Vector3& operator*=(double k) {
    return set(x * k, y * k, z * k);
  }

  Vector3& operator/=(double k) {
    return set(x / k, y / k, z / k);
  }

  double length() const {
    return sqrt(dot(*this));
  }

  Vector3 normalized() const {
    auto l = length();
    return Vector3(x / l, y / l, z / l);
  }

  double getDistanceTo(const Vector3& target) const {
    auto d = *this - target;
    return d.dot(d);
  }

  double dot(const Vector3& rside) const {
    return x * rside.x + y * rside.y + z * rside.z;
  }

  Vector3 cross(const Vector3& rside) const {
    auto x1 = y * rside.z - z * rside.y;
    auto y1 = z * rside.x - x * rside.z;
    auto z1 = x * rside.y - y * rside.x;
    return Vector3(x1, y1, z1);
  }

  double getCosWith(const Vector3& rside) const {
    auto d = dot(rside);
    auto l0 = length();
    auto l1 = rside.length();
    return d / (l0 * l1);
  }

  double getAngleWith(const Vector3& rside) const {
    return acos(getCosWith(rside));
  }

  std::string toString() const {
    return info();
  }

  virtual std::string info() const {
    char buf[32];
    snprintf(buf, sizeof(buf), "{%.4f, %.4f, %.4f}", x, y, z);
    return buf;
  }

};


double distance(const Vector3& u, const Vector3& v);

Vector3 operator*(double k, const Vector3& v);

class CoordSystem : public omnetpp::cObject {
 private:
  Vector3 ex = Vector3::EX;
  Vector3 ey = Vector3::EY;
  Vector3 ez = Vector3::EZ;

 public:
  static double VALIDATION_TOLERANCE;

  CoordSystem() {}

  CoordSystem(const Vector3& ex, const Vector3& ey, const Vector3& ez,
              bool check_cartesian = true, double tol = VALIDATION_TOLERANCE)
    : ex(ex.normalized()), ey(ey.normalized()), ez(ez.normalized())
  {
    if (check_cartesian)
      validateCartesianSystem(tol);
  }

  CoordSystem(const Vector3& forward, const Vector3& up,
              bool check_cartesian = true, double tol = VALIDATION_TOLERANCE) {
    ex = forward.normalized();
    ez = up.normalized();
    ey = ez.cross(ex);
    if (check_cartesian)
      validateCartesianSystem(tol);
  }

  CoordSystem(const CoordSystem& other)
    : ex(other.ex), ey(other.ey), ez(other.ez) {}

  virtual ~CoordSystem() {}

  virtual CoordSystem *dup() const {
    return new CoordSystem(*this);
  }

  CoordSystem& operator=(const CoordSystem& rside) {
    ex = rside.getEx();
    ey = rside.getEy();
    ez = rside.getEz();
    return *this;
  }

  const Vector3& getEx() const { return ex; }
  const Vector3& getEy() const { return ey; }
  const Vector3& getEz() const { return ex; }

  Vector3 relVecToAbs(const Vector3& v) const {
    return v.x * ex + v.y * ey + v.z * ez;
  }

  CoordSystem relCsToAbs(const CoordSystem& other) const {
    auto new_ex = relVecToAbs(other.ex);
    auto new_ey = relVecToAbs(other.ey);
    auto new_ez = relVecToAbs(other.ez);
    return CoordSystem(new_ex, new_ey, new_ez);
  }

 private:

  bool validateCartesianSystem(double tolerance, bool required = true);
};

}

#endif
