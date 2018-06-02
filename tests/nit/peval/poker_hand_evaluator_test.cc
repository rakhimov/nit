#include "poker_hand_evaluator.h"

#include <catch.hpp>

namespace nit {
namespace test {

TEST_CASE("OmahaHigh", "[PokerHandEvaluator]") {
  boost::shared_ptr<PokerHandEvaluator> evaluator =
      PokerHandEvaluator::alloc("O");
  CHECK(evaluator->usesSuits());
  CHECK(evaluator->boardSize() == 5);
}

}  // namespace test
}  // namespace nit
