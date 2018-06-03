/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#ifndef NIT_EVAL_DEUCE_TO_SEVEN_HAND_EVALUATOR_H_
#define NIT_EVAL_DEUCE_TO_SEVEN_HAND_EVALUATOR_H_

#include "poker_hand_evaluator.h"

namespace nit {

/**
 * A specialized hand evaluator for hold'em.  Not as slow.
 */
class DeuceToSevenHandEvaluator : public PokerHandEvaluator {
 public:
  DeuceToSevenHandEvaluator() = default;

  PokerHandEvaluation evaluateHand(const CardSet& hand,
                                   const CardSet&) const override {
    if (usesSuits())
      return PokerHandEvaluation(hand.evaluateLow2to7());
    else
      return PokerHandEvaluation(hand.evaluateRanksLow2to7());
  }

  PokerEvaluation evaluateRanks(
      const CardSet& hand, const CardSet& board = CardSet(0)) const override {
    return hand.evaluateRanksLow2to7();
  }

  PokerEvaluation evaluateSuits(
      const CardSet& hand, const CardSet& board = CardSet(0)) const override {
    return hand.evaluateSuitsLow2to7();
  }

  size_t handSize() const override { return 5; }
  size_t boardSize() const override { return 0; }
  size_t evaluationSize() const override { return 1; }
  size_t numDraws() const override { return m_numDraws; }
  void setNumDraws(size_t sz) override { m_numDraws = sz; }

 private:
  size_t m_numDraws{0};
};

}  // namespace nit

#endif  // NIT_EVAL_DEUCE_TO_SEVEN_HAND_EVALUATOR_H_
