#include <common/geometry.h>

namespace rfidsim {

const Vector3 Vector3::ZERO = Vector3(0, 0, 0);
const Vector3 Vector3::EX = Vector3(1, 0, 0);
const Vector3 Vector3::EY = Vector3(0, 1, 0);
const Vector3 Vector3::EZ = Vector3(0, 0, 1);

double distance(const Vector3& u, const Vector3& v) {
  return u.distance_to(v);
}

}
