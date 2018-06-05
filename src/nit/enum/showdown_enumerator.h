/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#ifndef NIT_ENUM_SHOWDOWN_ENUMERATOR_H_
#define NIT_ENUM_SHOWDOWN_ENUMERATOR_H_

#include <vector>

#include <nit/eval/poker_hand_evaluator.h>

#include "card_distribution.h"

namespace nit {

class ShowdownEnumerator {
 public:
  ShowdownEnumerator();

  /**
   * enumerate a poker scenario, with board support
   */
  std::vector<EquityResult> calculateEquity(
      const std::vector<CardDistribution>& dists, const CardSet& board,
      const PokerHandEvaluator& peval) const;
};

}  // namespace nit

#endif  // NIT_ENUM_SHOWDOWN_ENUMERATOR_H_
