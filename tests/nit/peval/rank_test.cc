#include "rank.h"

#include <cstring>

#include <catch.hpp>

using namespace nit;

const char* ranks = "23456789TJQKA";
const char* low_ranks = "23456789tjqka";

TEST_CASE("Rank.ConstructorChars", "[Rank]") {
  char rankstr[] = "?";

  // test the valid char range
  for (int i = 0; i < std::strlen(ranks); i++) {
    Rank r(ranks[i]);
    rankstr[0] = ranks[i];
    CHECK(r.str() == rankstr);
  }
}

TEST_CASE("Rank.ConstructorInts", "[Rank]") {
  char rankstr[] = "?";

  // test the valid int range
  for (int i = 0; i <= 12; i++) {
    Rank r(i);
    rankstr[0] = ranks[i];
    CHECK(r.str() == rankstr);
  }
}

TEST_CASE("Rank.ConstructorErrs", "[Rank]") {
  char rankstr[] = "?";

  // test the valid int range
  for (int i = 13; i < '2'; i++) {
    Rank r(i);
    CHECK(r.str() == rankstr);
  }
}
