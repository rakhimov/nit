/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#ifndef NIT_EVAL_OMAHA_HIGH_HAND_EVALUATOR_H_
#define NIT_EVAL_OMAHA_HIGH_HAND_EVALUATOR_H_

#include <boost/math/special_functions/binomial.hpp>

#include "holdem.h"
#include "poker_evaluation_tables.h"
#include "poker_hand_evaluator.h"

namespace nit {

/**
 * A specialized hand evaluator for omaha.  Not as slow.
 */
class OmahaHighHandEvaluator : public PokerHandEvaluator {
 public:
  static const int NUM_OMAHA_POCKET = 4;
  static const int NUM_OMAHA_FLOP = 3;
  static const int NUM_OMAHA_RIVER = 5;
  static const int NUM_OMAHA_HAND_USE = 2;
  static const int NUM_OMAHA_FLUSH_BOARD = 3;

  PokerHandEvaluation evaluateHand(const CardSet& hand,
                                   const CardSet& board) const override {
    PokerEvaluation eval[2];

    // generate the possible sub hands, player hand candidates are all
    // 4c2 combinations of hands cards,
    // board candidates are all Nc3 board candidates, where N is the size
    // of the board
    auto combos = boost::math::binomial_coefficient<double>(board.size(), 3);
    std::vector<CardSet> board_candidates(static_cast<size_t>(combos));
    std::vector<CardSet> hand_candidates(6);
    fillHands(hand_candidates, hand);
    fillBoards(board_candidates, board);

    for (auto& hand_candidate : hand_candidates)
      for (const auto& board_candidate : board_candidates) {
        PokerEvaluation e = (hand_candidate | board_candidate).evaluateHigh();
        if (e > eval[0])
          eval[0] = e;
      }

    return PokerHandEvaluation(eval[0]);
  }

  PokerEvaluation evaluateRanks(const CardSet& hand,
                                const CardSet& board) const override {
    PokerEvaluation eval;

    // generate the possible sub hands, player hand candidates are all
    // 4c2 combinations of hands cards,
    // board candidates are all Nc3 board candidates, where N is the size
    // of the board
    auto combos = boost::math::binomial_coefficient<double>(board.size(), 3);
    std::vector<CardSet> board_candidates(static_cast<size_t>(combos));
    std::vector<CardSet> hand_candidates(6);
    fillHands(hand_candidates, hand);
    fillBoards(board_candidates, board);
    for (size_t i = 0; i < hand_candidates.size(); i++)
      for (size_t j = 0; j < board_candidates.size(); j++) {
        CardSet meld(hand);
        meld.insertRanks(board);
        PokerEvaluation e = meld.evaluateHighRanks();
        if (e > eval)
          eval = e;
      }

    return eval;
  }

  PokerEvaluation evaluateSuits(const CardSet& hand,
                                const CardSet& board) const override {
    PokerEvaluation eval;

    // generate the possible sub hands, player hand candidates are all
    // 4c2 combinations of hands cards,
    // board candidates are all Nc3 board candidates, where N is the size
    // of the board
    auto combos = boost::math::binomial_coefficient<double>(board.size(), 3);
    std::vector<CardSet> board_candidates(static_cast<size_t>(combos));
    std::vector<CardSet> hand_candidates(6);
    fillHands(hand_candidates, hand);
    fillBoards(board_candidates, board);

    for (auto& hand_candidate : hand_candidates)
      for (const auto& board_candidate : board_candidates) {
        PokerEvaluation e =
            (hand_candidate | board_candidate).evaluateHighFlush();
        if (e > eval)
          eval = e;
      }

    return eval;
  }

  void fillHands(std::vector<CardSet>& candidates, const CardSet& cards) const {
    std::vector<CardSet> clist = cards.cardSets();
    candidates[0] = (clist[0] | clist[1]);
    candidates[1] = (clist[0] | clist[2]);
    candidates[2] = (clist[0] | clist[3]);
    candidates[3] = (clist[1] | clist[2]);
    candidates[4] = (clist[1] | clist[3]);
    candidates[5] = (clist[2] | clist[3]);
  }

  void fillBoards(std::vector<CardSet>& candidates,
                  const CardSet& cards) const {
    std::vector<CardSet> clist = cards.cardSets();
    switch (clist.size()) {
      case 5:
        candidates[4] = (clist[0] | clist[1] | clist[4]);
        candidates[5] = (clist[0] | clist[2] | clist[4]);
        candidates[6] = (clist[0] | clist[3] | clist[4]);
        candidates[7] = (clist[1] | clist[2] | clist[4]);
        candidates[8] = (clist[1] | clist[3] | clist[4]);
        candidates[9] = (clist[2] | clist[3] | clist[4]);
        // fall through

      case 4:
        candidates[1] = (clist[0] | clist[1] | clist[3]);
        candidates[2] = (clist[0] | clist[2] | clist[3]);
        candidates[3] = (clist[1] | clist[2] | clist[3]);
        // fall through

      case 3:
        candidates[0] = (clist[0] | clist[1] | clist[2]);
    }
  }

  size_t handSize() const override { return NUM_OMAHA_POCKET; }
  size_t boardSize() const override { return BOARD_SIZE; }
  size_t evaluationSize() const override { return 1; }
};

}  // namespace nit

#endif  // NIT_EVAL_OMAHA_HIGH_HAND_EVALUATOR_H_
