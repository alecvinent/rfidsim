#include <common/functions.h>
#include <iomanip>
#include <functional>
#include <algorithm>
#include <cctype>
#include <locale>
#include <string>

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

std::string ltrim(const std::string& s)
{
  std::string out_s = s;
  out_s.erase(out_s.begin(),
          std::find_if(out_s.begin(), out_s.end(),
                       std::not1(std::ptr_fun<int, int>(std::isspace))));
  return out_s;
}

std::string rtrim(const std::string& s)
{
  std::string out_s;
  out_s.erase(std::find_if(out_s.rbegin(), out_s.rend(),
                           std::not1(std::ptr_fun<int, int>(std::isspace))
                          ).base(),
          out_s.end());
  return out_s;
}

std::string trim(const std::string& s)
{
  return rtrim(ltrim(s));
}

std::string lower(const std::string& s)
{
  std::string out_s = s;
  std::transform(out_s.begin(), out_s.end(), out_s.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return out_s;
}

std::string upper(const std::string& s)
{
  auto out_s = s;
  std::transform(out_s.begin(), out_s.end(), out_s.begin(),
                 [](unsigned char c) { return std::toupper(c); });
  return out_s;
}

}
