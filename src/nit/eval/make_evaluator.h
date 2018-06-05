/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */

#ifndef NIT_EVAL_MAKE_EVALUATOR_H_
#define NIT_EVAL_MAKE_EVALUATOR_H_

#include <memory>
#include <string>

#include "poker_hand_evaluator.h"

namespace nit {

/**
 * Construction of subclasses must be done through this Factory.
 * The subclass is identified by the first letter of the input
 * string according to the table below
 *
 * supported games:
 * - 'h'    hold'em (or high if no board)
 * - 'k'    Kansas City lowball (2-7)
 * - 'l'    lowball (A-5)
 * - '3'    three card poker
 * - 'O'    omaha high
 * - 'r'    razz
 * - 's'    stud
 * - 'q'    stud high/low no qualifier
 * - 'd'    draw high
 * - 't'    triple draw lowball (2-7)
 * - 'T'    triple draw lowball (A-5)
 * - 'o'    omaha/high low
 * - 'e'    stud/8
 * - 'b'    badugi
 */
std::unique_ptr<PokerHandEvaluator> makeEvaluator(const std::string& strid);

}  // namespace nit

#endif  // NIT_EVAL_MAKE_EVALUATOR_H_
