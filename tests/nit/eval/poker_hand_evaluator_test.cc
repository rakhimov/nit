#include "poker_hand_evaluator.h"

#include <catch.hpp>

#include "make_evaluator.h"

namespace nit {
namespace test {

TEST_CASE("OmahaHigh", "[PokerHandEvaluator]") {
  std::shared_ptr<PokerHandEvaluator> evaluator = makeEvaluator("O");
  CHECK(evaluator->usesSuits());
  CHECK(evaluator->boardSize() == 5);
}

}  // namespace test
}  // namespace nit
