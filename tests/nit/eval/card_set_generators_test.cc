#include "card_set_generators.h"

#include <catch.hpp>

namespace nit {
namespace test {

TEST_CASE("CardSetSizes", "[CardSetGeneratorsTest]") {
  CHECK(nit::createCardSet(1).size() == 52);
  CHECK(nit::createCardSet(2).size() == 1326);
  CHECK(nit::createCardSet(3).size() == 22100);
}

TEST_CASE("CanonSetSizes", "[CardSetGeneratorsTest]") {
  CHECK(nit::createCardSet(1, Card::SUIT_CANONICAL).size() == 13);
  CHECK(nit::createCardSet(2, Card::SUIT_CANONICAL).size() == 169);
  CHECK(nit::createCardSet(3, Card::SUIT_CANONICAL).size() == 1755);
}

TEST_CASE("RankSetSizes", "[CardSetGeneratorsTest]") {
  CHECK(nit::createCardSet(1, Card::RANK).size() == 13);
  CHECK(nit::createCardSet(2, Card::RANK).size() == 91);
  CHECK(nit::createCardSet(3, Card::RANK).size() == 455);
}

}  // namespace test
}  // namespace nit
