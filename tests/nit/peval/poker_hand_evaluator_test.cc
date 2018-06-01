#include "poker_hand_evaluator.h"

#include <catch.hpp>

TEST_CASE("OmahaHigh", "[PokerHandEvaluator]") {
  using namespace nit;

  boost::shared_ptr<PokerHandEvaluator> evaluator =
      PokerHandEvaluator::alloc("O");
  CHECK(evaluator->usesSuits());
  CHECK(evaluator->boardSize() == 5);
}
