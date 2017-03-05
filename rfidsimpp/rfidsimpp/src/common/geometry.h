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

  double distance_to(const Vector3& target) const {
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

  double get_cosinus_with(const Vector3& rside) const {
    auto d = dot(rside);
    auto l0 = length();
    auto l1 = rside.length();
    return d / (l0 * l1);
  }

  double get_angle_with(const Vector3& rside) const {
    return acos(get_cosinus_with(rside));
  }

  std::string to_string() const {
    return info();
  }

  virtual std::string info() const {
    char buf[32];
    snprintf(buf, sizeof(buf), "{%.4f, %.4f, %.4f}", x, y, z);
    return buf;
  }

};

double distance(const Vector3& u, const Vector3& v);

}

#endif
