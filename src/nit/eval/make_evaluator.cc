/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */

#include "make_evaluator.h"

#include <nit/error.h>

#include "badugi_hand_evaluator.h"
#include "deuce_to_seven_hand_evaluator.h"
#include "draw_high_hand_evaluator.h"
#include "holdem_hand_evaluator.h"
#include "omaha_eight_hand_evaluator.h"
#include "omaha_high_hand_evaluator.h"
#include "razz_hand_evaluator.h"
#include "stud_eight_hand_evaluator.h"
#include "stud_hand_evaluator.h"
#include "universal_hand_evaluator.h"

namespace nit {

namespace detail {

template <class T, class... Ts>
std::unique_ptr<T> make_unique(Ts&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Ts>(args)...));
}

}  // namespace detail

std::unique_ptr<PokerHandEvaluator> makeEvaluator(const std::string& strid) {
  switch (strid[0]) {
    case 'h':  //     hold'em
    case 'H':  //     hold'em
      // ret = new UniversalHandEvaluator
      // (2,2,3,5,0,&CardSet::evaluateHigh, NULL);
      return detail::make_unique<HoldemHandEvaluator>();

    case 'k':  //     Kansas City lowball (2-7)
      return detail::make_unique<UniversalHandEvaluator>(
          1, 5, 0, 0, 0, &CardSet::evaluateLow2to7, nullptr);

    case 'l':  //     lowball (A-5)
      return detail::make_unique<UniversalHandEvaluator>(
          1, 5, 0, 0, 0, &CardSet::evaluateLowA5, nullptr);

    case '3':  //     three card poker
      return detail::make_unique<UniversalHandEvaluator>(
          1, 3, 0, 0, 0, &CardSet::evaluate3CP, nullptr);

    case 'O':  //     omaha high
      // ret.reset (new UniversalHandEvaluator
      // (4,4,3,5,2,&CardSet::evaluateHigh, NULL));
      return detail::make_unique<OmahaHighHandEvaluator>();

    case 'p':  //     pot limit
    case 'P':
      if (strid[2] == 'h' || strid[2] == 'H')  // plh/PLH
        return detail::make_unique<HoldemHandEvaluator>();

      if (strid[2] == 'o' || strid[2] == 'O')  // PLO
        return detail::make_unique<OmahaHighHandEvaluator>();

      throw LogicError("no compatible pot limit game available");

      //
      // For the draw games, we need to be able to evaluate 1 card to
      // determine the bring in on 3rd street

    case 'r':  //     razz
      // ret.reset (new UniversalHandEvaluator
      // (1,7,0,0,0,&CardSet::evaluateLowA5, NULL));
      return detail::make_unique<RazzHandEvaluator>();

    case 's':  //     stud
      // ret.reset (new UniversalHandEvaluator
      // (1,7,0,0,0,&CardSet::evaluateHigh, NULL));
      return detail::make_unique<StudHandEvaluator>();

    case 'q':  //     stud high/low no qualifier
      return detail::make_unique<UniversalHandEvaluator>(
          1, 7, 0, 0, 0, &CardSet::evaluateHigh, &CardSet::evaluateLowA5);

    case 'd':  //     draw high
    case 'D':  //     draw high
      // ret.reset (new UniversalHandEvaluator
      // (1,5,0,0,0,&CardSet::evaluateHigh, NULL));
      return detail::make_unique<DrawHighHandEvaluator>();

    case 't':  //     triple draw lowball (2-7)
      // ret.reset (new UniversalHandEvaluator
      // (1,5,0,0,0,&CardSet::evaluateLow2to7, NULL));
      return detail::make_unique<DeuceToSevenHandEvaluator>();

    case 'T':  //     triple draw lowball (A-5)
      return detail::make_unique<UniversalHandEvaluator>(
          1, 5, 0, 0, 0, &CardSet::evaluateLowA5, nullptr);

    case 'o':  //     omaha/high low
      // ret.reset (new UniversalHandEvaluator (4,4,3,5,2,
      // &CardSet::evaluateHigh, &CardSet::evaluate8LowA5));
      return detail::make_unique<OmahaEightHandEvaluator>();

    case 'e':  //     stud/8
      // ret.reset (new UniversalHandEvaluator
      // (1,7,0,0,0,&CardSet::evaluateHigh, &CardSet::evaluate8LowA5));
      return detail::make_unique<StudEightHandEvaluator>();

    case 'b':  //     badugi
      // ret.reset (new UniversalHandEvaluator
      // (0,52,0,0,0,&CardSet::evaluateBadugi,NULL));
      return detail::make_unique<BadugiHandEvaluator>();
  }
  throw LogicError("No evaluator for poker type.") << errinfo_value(strid);
}

}  // namespace nit
