#include <iomanip>
#include <common/functions.h>

namespace rfidsim {

std::string toHexString(const std::vector<uint8_t>& v)
{
  std::stringstream ss;
  for (auto b: v) {
    ss << std::setfill('0') << std::setw(2) << std::hex << b;
  }
  return ss.str();
}

}
