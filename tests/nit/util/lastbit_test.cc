#include "lastbit.h"

#define CATCH_CONFIG_MAIN

#include <catch.hpp>

TEST_CASE("lastbit32", "[lastbit]") {
  // There are uint16_t, uint32_t and uint64_t overloads of lastbit.
  // Explicitly cast arg so we know which we are testing.
  CHECK(lastbit(1u) == 0);
  CHECK(lastbit(2u) == 1);
  CHECK(lastbit(3u) == 0);
  CHECK(lastbit(4u) == 2);
  CHECK(lastbit(31u) == 0);
  CHECK(lastbit(32u) == 5);
  CHECK(lastbit(36u) == 2);

  for (int i = 1; i < 32; i++) {
    CHECK(lastbit((1u << i) - 1) == 0);
    CHECK(lastbit((1u << i)) == i);
    CHECK(lastbit((1u << i) + 1) == 0);
    // Set a higher bit also.
    if (i < 30) {
      CHECK(lastbit((1u << i) | (1u << (i + 2))) == i);
    }
  }
}

TEST_CASE("lastbit16", "[lastbit]") {
  // There are uint16_t, uint32_t and uint64_t overloads of lastbit.
  // Explicitly cast arg so we know which we are testing.
  CHECK(lastbit(uint16_t(1)) == 0);
  CHECK(lastbit(uint16_t(2)) == 1);
  CHECK(lastbit(uint16_t(3)) == 0);
  CHECK(lastbit(uint16_t(4)) == 2);
  CHECK(lastbit(uint16_t(31)) == 0);
  CHECK(lastbit(uint16_t(32)) == 5);
  CHECK(lastbit(uint16_t(36)) == 2);

  for (int i = 1; i < 16; i++) {
    CHECK(lastbit(uint16_t((UINT16_C(1) << i) - 1)) == 0);
    CHECK(lastbit(uint16_t((UINT16_C(1) << i))) == i);
    CHECK(lastbit(uint16_t((UINT16_C(1) << i) + 1)) == 0);
    if (i < 14) {
      CHECK(lastbit(uint16_t((UINT16_C(1) << i) | (UINT16_C(1) << (i + 2)))) ==
            i);
      CHECK(lastbit((1u << i) | (1u << (i + 2))) == i);
    }
  }
}

TEST_CASE("lastbit 64 overload", "[lastbit]") {
  // There are uint16_t, uint32_t and uint64_t overloads of lastbit.
  // Explicitly cast arg so we know which we are testing.
  CHECK(lastbit(uint64_t(1)) == 0);
  CHECK(lastbit(uint64_t(2)) == 1);
  CHECK(lastbit(uint64_t(3)) == 0);
  CHECK(lastbit(uint64_t(4)) == 2);
  CHECK(lastbit(uint64_t(31)) == 0);
  CHECK(lastbit(uint64_t(32)) == 5);
  CHECK(lastbit(uint64_t(36)) == 2);

  for (int i = 1; i < 64; i++) {
    CHECK(lastbit(uint64_t((1ull << i) - 1)) == 0);
    CHECK(lastbit((uint64_t(1ull << i))) == i);
    CHECK(lastbit((uint64_t(1ull << i)) + 1) == 0);
    if (i < 62) {
      CHECK(lastbit(uint64_t((1ull << i) | (1ull << (i + 2)))) == i);
    }
  }
}

TEST_CASE("lastbit64", "[lastbit]") {
  // No overloads for this function.
  CHECK(lastbit64(1) == 0);
  CHECK(lastbit64(2) == 1);
  CHECK(lastbit64(3) == 0);
  CHECK(lastbit64(4) == 2);
  CHECK(lastbit64(31) == 0);
  CHECK(lastbit64(32) == 5);
  CHECK(lastbit64(36) == 2);

  for (int i = 1; i < 64; i++) {
    CHECK(lastbit64((1ull << i) - 1) == 0);
    CHECK(lastbit64((1ull << i)) == i);
    CHECK(lastbit64((1ull << i) + 1) == 0);
    if (i < 62) {
      CHECK(lastbit64((1ull << i) | (1ull << (i + 2))) == i);
    }
  }
}
