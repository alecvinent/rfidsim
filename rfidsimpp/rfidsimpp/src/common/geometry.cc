#include <common/geometry.h>

namespace rfidsim {

const Vector3 Vector3::ZERO = Vector3(0, 0, 0);
const Vector3 Vector3::EX = Vector3(1, 0, 0);
const Vector3 Vector3::EY = Vector3(0, 1, 0);
const Vector3 Vector3::EZ = Vector3(0, 0, 1);

double distance(const Vector3& u, const Vector3& v) {
  return u.getDistanceTo(v);
}

Vector3 operator*(double k, const Vector3& v) {
  return v * k;
}


double CoordSystem::VALIDATION_TOLERANCE = 1e-5;

bool CoordSystem::validateCartesianSystem(double tolerance, bool required)
{
  bool err = std::abs(ex.dot(ey)) > tolerance
          || std::abs(ex.dot(ez)) > tolerance
          || std::abs(ey.dot(ez)) > tolerance;
  if (err) {
    if (required) {
      throw omnetpp::cRuntimeError(
              "not Cartesian system: ex=%s, ey=%s, ez=%s",
              ex.info().c_str(), ey.info().c_str(), ez.info().c_str());
    } else {
      return false;
    }
  }
  return true;
}

}
