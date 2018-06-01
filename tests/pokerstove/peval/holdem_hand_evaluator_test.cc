#include "holdem_hand_evaluator.h"

#include <iostream>

#include <catch.hpp>

using namespace pokerstove;
using namespace std;

TEST_CASE("Construct Holdem", "[HoldemHandEvaluator]") {
  HoldemHandEvaluator eval;
  CHECK(eval.usesSuits() == true);
  CHECK(eval.boardSize() == 5);
}

TEST_CASE("RankEval Holdem", "[HoldemHandEvaluator]") {
  HoldemHandEvaluator heval;
  CardSet hand("2c3c");
  CardSet board("2c3c4c");
  PokerEvaluation eval = heval.evaluateRanks(hand, board);
  CHECK(eval.type() == TWO_PAIR);
  CHECK(eval.majorRank() == Rank("3"));
  CHECK(eval.minorRank() == Rank("2"));
}
