/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#ifndef NIT_EVAL_HOLDEM_H_
#define NIT_EVAL_HOLDEM_H_

namespace nit {

// Facts about holdem.
const unsigned int PREFLOP = 0;
const unsigned int FLOP = 1;
const unsigned int TURN = 2;
const unsigned int RIVER = 3;
const unsigned int BOARD_SIZE = 5;
const unsigned int NUM_FLOP_CARDS = 3;
const unsigned int NUM_TURN_CARDS = 4;
const unsigned int NUM_RIVER_CARDS = 5;
const unsigned int NUM_HOLDEM_POCKET = 2;
const unsigned int NUM_HOLDEM_ROUNDS = 4;

}  // namespace nit

#endif  // NIT_EVAL_HOLDEM_H_
