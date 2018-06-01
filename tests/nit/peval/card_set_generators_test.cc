#include "card_set_generators.h"

#include <catch.hpp>

using namespace pokerstove;

TEST_CASE("CardSetSizes", "[CardSetGeneratorsTest]") {
  CHECK(pokerstove::createCardSet(1).size() == 52);
  CHECK(pokerstove::createCardSet(2).size() == 1326);
  CHECK(pokerstove::createCardSet(3).size() == 22100);
}

TEST_CASE("CanonSetSizes", "[CardSetGeneratorsTest]") {
  CHECK(pokerstove::createCardSet(1, Card::SUIT_CANONICAL).size() == 13);
  CHECK(pokerstove::createCardSet(2, Card::SUIT_CANONICAL).size() == 169);
  CHECK(pokerstove::createCardSet(3, Card::SUIT_CANONICAL).size() == 1755);
}

TEST_CASE("RankSetSizes", "[CardSetGeneratorsTest]") {
  CHECK(pokerstove::createCardSet(1, Card::RANK).size() == 13);
  CHECK(pokerstove::createCardSet(2, Card::RANK).size() == 91);
  CHECK(pokerstove::createCardSet(3, Card::RANK).size() == 455);
}
