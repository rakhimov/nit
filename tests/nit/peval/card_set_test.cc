#include "card_set.h"

#include <catch.hpp>

TEST_CASE("StringConstructorToString", "[CardSetTest]") {
  using namespace nit;

  CHECK(CardSet("Ac").str() == "Ac");
  CHECK(CardSet("As").str() == "As");
  CHECK(CardSet("qh").str() == "Qh");
  CHECK(CardSet("Td").str() == "Td");
  CHECK(CardSet("td").str() == "Td");
  // EXPECT_THROW(CardSet("10d"));
  // EXPECT_THROW(CardSet("AcAc"));
}

TEST_CASE("StringConstructorSize", "[CardSetTest]") {
  using namespace nit;

  CHECK(CardSet("Ac").size() == 1);
  CHECK(CardSet("qh").size() == 1);
  CHECK(CardSet("2h3h4h5h6h7h8h9hThJhQhKhAh").size() == 13);

  // duplicate cards rolls back all parsing
  CHECK(CardSet("2h2h").size() == 0);
}

TEST_CASE("Canonize", "[CardSetTest]") {
  using namespace nit;

  CHECK(CardSet("2c3c").canonize() == CardSet("2c3c"));
  CHECK(CardSet("2s3s").canonize() == CardSet("2c3c"));
  CHECK(CardSet("2s3h4c").canonize() == CardSet("4c3d2h"));
}

TEST_CASE("CanonizeRanks", "[CardSetTest]") {
  using namespace nit;

  const CardSet AceCanon1("Ac");
  const CardSet AceCanon2("AcAd");
  const CardSet AceCanon3("AcAdAh");
  const CardSet AceCanon4("AcAdAhAs");

  // all combinations of one
  CHECK(CardSet("Ac").canonizeRanks() == AceCanon1);
  CHECK(CardSet("Ad").canonizeRanks() == AceCanon1);
  CHECK(CardSet("Ah").canonizeRanks() == AceCanon1);
  CHECK(CardSet("As").canonizeRanks() == AceCanon1);

  // all combinations of two
  CHECK(CardSet("AcAd").canonizeRanks() == AceCanon2);
  CHECK(CardSet("AcAh").canonizeRanks() == AceCanon2);
  CHECK(CardSet("AcAs").canonizeRanks() == AceCanon2);
  CHECK(CardSet("AdAh").canonizeRanks() == AceCanon2);
  CHECK(CardSet("AdAs").canonizeRanks() == AceCanon2);
  CHECK(CardSet("AhAs").canonizeRanks() == AceCanon2);

  // all combinations of three
  CHECK(CardSet("AcAdAh").canonizeRanks() == AceCanon3);
  CHECK(CardSet("AcAdAs").canonizeRanks() == AceCanon3);
  CHECK(CardSet("AcAhAs").canonizeRanks() == AceCanon3);
  CHECK(CardSet("AdAhAs").canonizeRanks() == AceCanon3);

  // the one combination of four
  CHECK(CardSet("AcAdAhAs").canonizeRanks() == AceCanon4);

  // some multi card exampes
  CHECK(CardSet("Ac2d3h4d5s").canonizeRanks() == CardSet("Ac2c3c4c5c"));
  CHECK(CardSet("Ac2d3h4d5sAh").canonizeRanks() == CardSet("AcAd2c3c4c5c"));
  CHECK(CardSet("Ac2d3h4d5sAh").canonizeRanks() == CardSet("AcAd2c3c4c5c"));
}

TEST_CASE("fill", "[CardSetTest]") {
  using namespace nit;
  CardSet all;
  all.fill();
  CHECK(all.size() == STANDARD_DECK_SIZE);
}
