/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#ifndef NIT_PENUM_SHOWDOWN_ENUMERATOR_H_
#define NIT_PENUM_SHOWDOWN_ENUMERATOR_H_

#include <memory>
#include <vector>

#include <nit/peval/poker_hand_evaluator.h>

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
      std::shared_ptr<PokerHandEvaluator> peval) const;
};

}  // namespace nit

#endif  // NIT_PENUM_SHOWDOWN_ENUMERATOR_H_
