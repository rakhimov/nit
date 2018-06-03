/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#ifndef NIT_PEVAL_DRAW_HIGH_HAND_EVALUATOR_H_
#define NIT_PEVAL_DRAW_HIGH_HAND_EVALUATOR_H_

#include "poker_hand_evaluator.h"

namespace nit {

/**
 * A specialized hand evaluator for hold'em.  Not as slow.
 */
class DrawHighHandEvaluator : public PokerHandEvaluator {
 public:
  DrawHighHandEvaluator() = default;

  PokerHandEvaluation evaluateHand(const CardSet& hand,
                                   const CardSet&) const override {
    return PokerHandEvaluation(hand.evaluateHigh());
  }

  PokerEvaluation evaluateRanks(
      const CardSet& hand, const CardSet& board = CardSet(0)) const override {
    return hand.evaluateHighRanks();
  }

  PokerEvaluation evaluateSuits(
      const CardSet& hand, const CardSet& board = CardSet(0)) const override {
    return hand.evaluateHighFlush();
  }

  size_t handSize() const override { return _handSize; }
  size_t boardSize() const override { return 0; }
  size_t evaluationSize() const override { return 1; }

  virtual void setHandSize(size_t sz) { _handSize = sz; }

 private:
  size_t _handSize{5};
};

}  // namespace nit

#endif  // NIT_PEVAL_DRAW_HIGH_HAND_EVALUATOR_H_
