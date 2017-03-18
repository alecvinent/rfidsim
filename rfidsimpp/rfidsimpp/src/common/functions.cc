#include <common/functions.h>
#include <iomanip>

using namespace omnetpp;

namespace rfidsim {

std::string toHexString(const std::vector<uint8_t>& v)
{
  std::stringstream ss;
  for (auto b: v) {
    ss << std::setfill('0') << std::setw(2) << std::hex << b;
  }
  return ss.str();
}

inline uint8_t parseHexDigit(char digit)
{
  if (digit >= '0' && digit <= '9')
    return digit - '0';
  else if (digit >= 'a' && digit <= 'f')
    return 10 + (digit - 'a');
  else if (digit >= 'A' && digit <= 'F')
    return 10 + (digit - 'A');
  else
    throw cRuntimeError("char '%c' is not a hex digit", digit);
}

std::vector<uint8_t> parseHexString(const std::string& s)
{
  std::vector<uint8_t> bytes;

  if (s.length() % 2 != 0)
    throw cRuntimeError("'%s' is not a well-formed hex string", s.c_str());

  for (unsigned i = 0; i < s.length(); i += 2)
  {
    uint8_t msb = parseHexDigit(s[i]);
    uint8_t lsb = parseHexDigit(s[i + 1]);
    bytes.push_back(msb * 0x10 + lsb);
  }
  return bytes;
}

}
