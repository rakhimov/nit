/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#ifndef NIT_EVAL_HOLDEM_HAND_EVALUATOR_H_
#define NIT_EVAL_HOLDEM_HAND_EVALUATOR_H_

#include "holdem.h"
#include "poker_hand_evaluator.h"

namespace nit {

/**
 * A specialized hand evaluator for hold'em.  Not as slow.
 */
class HoldemHandEvaluator : public PokerHandEvaluator {
 public:
  PokerHandEvaluation evaluateHand(const CardSet& hand,
                                   const CardSet& board) const override {
    // if (hand.size () != NUM_HOLDEM_POCKET)
    // throw std::invalid_argument ("HHE: incorrect number of pocket
    // cards");
    CardSet h = hand;
    h.insert(board);
    return PokerHandEvaluation(h.evaluateHigh());
  }

  PokerEvaluation evaluateRanks(
      const CardSet& hand, const CardSet& board = CardSet(0)) const override {
    CardSet h = hand;
    h.insertRanks(board);
    return h.evaluateHighRanks();
  }

  PokerEvaluation evaluateSuits(
      const CardSet& hand, const CardSet& board = CardSet(0)) const override {
    CardSet h = hand;
    h.insert(board);
    return h.evaluateHighFlush();
  }

  size_t handSize() const override { return NUM_HOLDEM_POCKET; }
  size_t boardSize() const override { return BOARD_SIZE; }
  size_t evaluationSize() const override { return 1; }
};

}  // namespace nit

#endif  // NIT_EVAL_HOLDEM_HAND_EVALUATOR_H_
