#ifndef COMMON_FUNCTIONS_H_
#define COMMON_FUNCTIONS_H_

#include <omnetpp.h>
#include <cstdint>

namespace rfidsim {

template<typename T>
T maximum(T v) {
  return v;
}

template<typename T, typename... Args>
T maximum(T first, Args... args) {
  T second = maximum(args...);
  return first < second ? second : first;
}

template<typename T>
T minimum(T v) {
  return v;
}

template<typename T, typename... Args>
T minimum(T first, Args... args) {
  T second = minimum(args...);
  return first > second ? second : first;
}

std::string toHexString(const std::vector<uint8_t>& v);
std::vector<uint8_t> parseHexString(const std::string& s);

std::string ltrim(const std::string& s);
std::string rtrim(const std::string& s);
std::string trim(const std::string& s);
std::string lower(const std::string& s);
std::string upper(const std::string& s);


}

#endif /* COMMON_FUNCTIONS_H_ */
