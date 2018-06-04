#ifndef NIT_UTIL_LASTBIT_H_
#define NIT_UTIL_LASTBIT_H_

#include <cassert>
#include <cstdint>

#include <nit/util/utypes.h>

#ifdef _MSC_VER
#include <intrin.h>
#include <windows.h>

#pragma intrinsic(_BitScanForward)
#ifndef _M_IX86
#pragma intrinsic(_BitScanForward64)
#endif

#endif

inline int lastbit(uint32_t v) {
  assert(v != 0);
#if defined(__GNUC__)
  return __builtin_ctz(v);

#elif defined(_MSC_VER)
  DWORD trailing_zero = 0;
  auto ret = _BitScanForward(&trailing_zero, v);
  assert(ret);
  return trailing_zero;

#else
#error "No implementation with bit-ops intrinsics!"
#endif
}

inline int lastbit(uint64_t v) {
  assert(v != 0);
#if defined(__GNUC__)
  return __builtin_ctzll(v);

#elif defined(_MSC_VER)

#ifdef _M_IX86
  auto lower = static_cast<uint32_t>(v);
  if (lower)
    return lastbit(lower);
  auto upper = static_cast<uint32_t>(v >> 32);
  return lastbit(upper) + 32;
#else
  DWORD trailing_zero = 0;
  auto ret = _BitScanForward64(&trailing_zero, v);
  assert(ret);
  return trailing_zero;
#endif

#else
#error "No implementation with bit-ops intrinsics!"
#endif
}

inline int lastbit(uint16_t v) { return lastbit(static_cast<uint32_t>(v)); }
inline int lastbit64(uint64_t v) { return lastbit(v); }

#endif  // NIT_UTIL_LASTBIT_H_
