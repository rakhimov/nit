#include "suit.h"

#include <cstring>

#include <catch.hpp>

namespace nit {
namespace test {

const char* suits = "cdhsCDHS";

TEST_CASE("Suit.ConstructorChars", "[Suit]") {
  char suitstr[] = "?";

  // test the valid char range
  for (int i = 0; i < std::strlen(suits); i++) {
    Suit s(suits[i]);
    suitstr[0] = suits[i];
    CHECK_THAT(s.str(), Catch::Equals(suitstr, Catch::CaseSensitive::No));
  }
}

TEST_CASE("Suit.ConstructorInts", "[Suit]") {
  char suitstr[] = "?";

  // test the valid int range
  for (int i = 0; i <= 3; i++) {
    Suit s(i);
    suitstr[0] = suits[i];
    CHECK(s.str() == suitstr);
  }
}

TEST_CASE("Suit.ConstructorErrs", "[Suit]") {
  char suitstr[] = "?";

  // test the invalid int range
  for (int i = 4; i < 'C'; i++) {
    Suit s(i);
    CHECK(s.str() == suitstr);
  }
}

}  // namespace test
}  // namespace nit
