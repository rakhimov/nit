#ifndef NIT_UTIL_UTYPES_H_
#define NIT_UTIL_UTYPES_H_

#include <cstdint>

#include <string>

using uint = unsigned int;
// typedef unsigned char ubyte;

const uint64_t ONE64 = 1;

/**
 * Produce a string corresponding to the bits in the data.
 * A space separates every 8 bits.
 */
template <typename T>
std::string toBitString(T t) {
  std::string ret;
  const int BYTE_SIZE = 8;
  for (unsigned int i = 0; i < sizeof(T) * BYTE_SIZE; i++) {
    if ((i % BYTE_SIZE) == 0)
      ret += " ";
    if (t >> i & 0x01)
      ret += "1";
    else
      ret += "0";
  }
  return ret;
}

#endif  // NIT_UTIL_UTYPES_H_
