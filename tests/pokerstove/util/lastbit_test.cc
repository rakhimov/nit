#include "lastbit.h"

#define CATCH_CONFIG_MAIN

#include <catch.hpp>

TEST_CASE("firstbit64", "[lastbit]") {
  // firstbit arg is uint64_t
  // There are no overloads, so we don't have to worry about which
  // one is being tested.
  CHECK(firstbit(0) == 0);
  CHECK(firstbit(1) == 0);
  CHECK(firstbit(2) == 1);
  CHECK(firstbit(3) == 1);
  CHECK(firstbit(4) == 2);
  CHECK(firstbit(31) == 4);
  CHECK(firstbit(32) == 5);

  for (int i = 1; i < 64; i++) {
    CHECK(firstbit((UINT64_C(1) << i) - 1) == (i - 1));
    CHECK(firstbit((UINT64_C(1) << i)) == i);
  }
}

TEST_CASE("lastbit32", "[lastbit]") {
  // There are uint16_t, uint32_t and uint64_t overloads of lastbit.
  // Explicitly cast arg so we know which we are testing.
  CHECK(lastbit(uint32_t(0)) == 0);
  CHECK(lastbit(uint32_t(1)) == 0);
  CHECK(lastbit(uint32_t(2)) == 1);
  CHECK(lastbit(uint32_t(3)) == 0);
  CHECK(lastbit(uint32_t(4)) == 2);
  CHECK(lastbit(uint32_t(31)) == 0);
  CHECK(lastbit(uint32_t(32)) == 5);
  CHECK(lastbit(uint32_t(36)) == 2);

  for (int i = 1; i < 32; i++) {
    CHECK(lastbit(uint32_t((UINT32_C(1) << i) - 1)) == 0);
    CHECK(lastbit(uint32_t((UINT32_C(1) << i))) == i);
    CHECK(lastbit(uint32_t((UINT32_C(1) << i) + 1)) == 0);
    // Set a higher bit also.
    if (i < 30) {
      CHECK(lastbit(uint32_t((UINT32_C(1) << i) | (UINT32_C(1) << (i + 2)))) ==
            i);
    }
  }
}

TEST_CASE("lastbit16", "[lastbit]") {
  // There are uint16_t, uint32_t and uint64_t overloads of lastbit.
  // Explicitly cast arg so we know which we are testing.
  CHECK(lastbit(uint16_t(0)) == 0);
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
    }
  }
}

TEST_CASE("lastbit 64 overload", "[lastbit]") {
  // There are uint16_t, uint32_t and uint64_t overloads of lastbit.
  // Explicitly cast arg so we know which we are testing.
  CHECK(lastbit(uint64_t(0)) == 0);
  CHECK(lastbit(uint64_t(1)) == 0);
  CHECK(lastbit(uint64_t(2)) == 1);
  CHECK(lastbit(uint64_t(3)) == 0);
  CHECK(lastbit(uint64_t(4)) == 2);
  CHECK(lastbit(uint64_t(31)) == 0);
  CHECK(lastbit(uint64_t(32)) == 5);
  CHECK(lastbit(uint64_t(36)) == 2);

  for (int i = 1; i < 64; i++) {
    CHECK(lastbit(uint64_t((UINT64_C(1) << i) - 1)) == 0);
    CHECK(lastbit(uint64_t((UINT64_C(1) << i))) == i);
    CHECK(lastbit(uint64_t((UINT64_C(1) << i) + 1)) == 0);
    if (i < 62) {
      CHECK(lastbit(uint64_t((UINT64_C(1) << i) | (UINT64_C(1) << (i + 2)))) ==
            i);
    }
  }
}

TEST_CASE("lastbit64", "[lastbit]") {
  // No overloads for this function.
  CHECK(lastbit64(0) == 0);
  CHECK(lastbit64(1) == 0);
  CHECK(lastbit64(2) == 1);
  CHECK(lastbit64(3) == 0);
  CHECK(lastbit64(4) == 2);
  CHECK(lastbit64(31) == 0);
  CHECK(lastbit64(32) == 5);
  CHECK(lastbit64(36) == 2);

  for (int i = 1; i < 64; i++) {
    CHECK(lastbit64((UINT64_C(1) << i) - 1) == 0);
    CHECK(lastbit64((UINT64_C(1) << i)) == i);
    CHECK(lastbit64((UINT64_C(1) << i) + 1) == 0);
    if (i < 62) {
      CHECK(lastbit64((UINT64_C(1) << i) | (UINT64_C(1) << (i + 2))) == i);
    }
  }
}
