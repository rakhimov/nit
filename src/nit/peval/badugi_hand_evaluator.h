/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#ifndef NIT_PEVAL_BADUGI_HAND_EVALUATOR_H_
#define NIT_PEVAL_BADUGI_HAND_EVALUATOR_H_

#include "poker_hand_evaluator.h"

namespace nit {

/**
 * A specialized hand evaluator for hold'em.  Not as slow.
 */
class BadugiHandEvaluator : public PokerHandEvaluator {
 public:
  BadugiHandEvaluator() = default;

  PokerHandEvaluation evaluateHand(const CardSet& hand,
                                   const CardSet&) const override {
    return PokerHandEvaluation(hand.evaluateBadugi());
  }

  PokerEvaluation evaluateRanks(
      const CardSet& hand, const CardSet& board = CardSet(0)) const override {
    throw std::runtime_error(
        "BadugiHandEvaluator::evaluateRanks, not implemented");
  }

  PokerEvaluation evaluateSuits(
      const CardSet& hand, const CardSet& board = CardSet(0)) const override {
    throw std::runtime_error(
        "BadugiHandEvaluator::evaluateSuits, not implemented");
  }

  size_t handSize() const override { return 4; }
  size_t boardSize() const override { return 0; }
  size_t evaluationSize() const override { return 1; }
  size_t numDraws() const override { return m_numDraws; }
  void setNumDraws(size_t sz) override { m_numDraws = sz; }

 private:
  size_t m_numDraws{0};
};

}  // namespace nit

#endif  // NIT_PEVAL_BADUGI_HAND_EVALUATOR_H_
