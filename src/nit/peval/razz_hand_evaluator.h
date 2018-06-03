/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#ifndef NIT_PEVAL_RAZZ_HAND_EVALUATOR_H_
#define NIT_PEVAL_RAZZ_HAND_EVALUATOR_H_

#include "poker_hand_evaluator.h"

namespace nit {

/**
 * A specialized hand evaluator for hold'em.  Not as slow.
 */
class RazzHandEvaluator : public PokerHandEvaluator {
 public:
  PokerHandEvaluation evaluateHand(const CardSet& hand,
                                   const CardSet&) const override {
    return PokerHandEvaluation(hand.evaluateLowA5());
  }

  PokerEvaluation evaluateRanks(
      const CardSet& hand, const CardSet& board = CardSet(0)) const override {
    return hand.evaluateLowA5();
  }

  bool usesSuits() const override { return false; }

  size_t handSize() const override { return 7; }
  size_t boardSize() const override { return 0; }
  size_t evaluationSize() const override { return 1; }
};

}  // namespace nit

#endif  // NIT_PEVAL_RAZZ_HAND_EVALUATOR_H_
