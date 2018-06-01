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
  virtual PokerHandEvaluation evaluateHand(const CardSet& hand,
                                           const CardSet&) const {
    return PokerHandEvaluation(hand.evaluateLowA5());
  }

  virtual PokerEvaluation evaluateRanks(
      const CardSet& hand, const CardSet& board = CardSet(0)) const {
    return hand.evaluateLowA5();
  }

  virtual bool usesSuits() const { return false; }

  virtual size_t handSize() const { return 7; }
  virtual size_t boardSize() const { return 0; }
  virtual size_t evaluationSize() const { return 1; }
};

}  // namespace nit

#endif  // NIT_PEVAL_RAZZ_HAND_EVALUATOR_H_
