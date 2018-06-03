/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#ifndef NIT_PEVAL_UNIVERSAL_HAND_EVALUATOR_H_
#define NIT_PEVAL_UNIVERSAL_HAND_EVALUATOR_H_

// This is a rule based hand evaluator which can be used to construct
// evalutors for most kinds of poker evaluation types.  Because it is
// a generic code base, it is fairly slow, but allows for rule driven
// poker games to be evaluated, and thus is more flexible.
//
// The current configurable parameters are:
//  hero hand: min/max cards
//  board: min/max cards
//  eval a: high/low/227/A25/Badugi/3CP
//  eval b: high/low/227/A25/Badugi/3CP

#include <vector>

#include <nit/util/combinations.h>

#include "card.h"
#include "card_set.h"
#include "poker_hand_evaluator.h"

namespace nit {

using evalFunction = PokerEvaluation (CardSet::*)() const;

/**
 * A generic poker game hand evaluator, from which nearly all poker evaluators
 * can be made.  This class is used as the default class type in the
 * PokerHandEvaluator factory.
 */
class UniversalHandEvaluator : public PokerHandEvaluator {
 public:
  /**
   * There is only one constructor which takes all the parameters.
   * The evaluation varieties are specified by pointer to member
   * functions to the CardSet class.
   *
   * @param heromin smallest possible hero hand
   * @param heromax largest possible hero hand
   * @param boardmin smallest possible board
   * @param boardmax largest possible board
   * @param herouse num cards hero must use (e.g. omaha->2)
   * @param evalA primary evaluation function, all games must have
   * @param evalB secondary evaluation function, most games use none (null)
   */
  UniversalHandEvaluator(int heromin, int heromax, int boardmin, int boardmax,
                         int herouse, evalFunction evalA, evalFunction evalB)

      : PokerHandEvaluator(),
        m_heromin(heromin),
        m_heromax(heromax),
        m_boardmin(boardmin),
        m_boardmax(boardmax),
        m_herouse(herouse),
        m_evalA(evalA),
        m_evalB(evalB) {
    if (evalA == evalFunction(nullptr))
      throw std::invalid_argument(
          "UnivHandEval: first evaluator (A) must be non-null");

    if (evalB == evalFunction(nullptr))
      m_evalsperhand = 1;
    else
      m_evalsperhand = 2;
  }

  size_t handSize() const override { return m_heromax; }
  size_t boardSize() const override { return m_boardmax; }

  size_t evaluationSize() const override {
    if (m_evalB == evalFunction(nullptr))
      return 1;
    return 2;
  }

  PokerHandEvaluation evaluateHand(const CardSet& hand,
                                   const CardSet& board) const override {
    PokerEvaluation eval[2];

    // check to see if the input hand is consistent with the game
    if (hand.size() < m_heromin || hand.size() > m_heromax)
      throw std::invalid_argument(
          std::string("UnivHandEval: " + std::to_string(hand.size()) +
                      ": invalid number of pocket cards"));
    CardSet h = hand;

    std::vector<CardSet> hand_candidates;
    std::vector<CardSet> board_candidates;
    std::vector<CardSet> eval_candidates;

    // now check the board, it's a distribution
    size_t bz = board.size();
    if ((bz < m_boardmin && bz > 0) || bz > m_boardmax)
      throw std::invalid_argument(
          std::string("UnivHandEval: " + std::to_string(board.size()) +
                      " unsupported number of board cards"));

    // generate the possible sub parts, the reference example is omaha
    // where a player must use two cards from their hand, and three
    // from the board.
    fillSubsets(hand_candidates, m_herouse, h);
    fillSubsets(board_candidates, boardSize() - m_herouse, board);

    // combine the subset candidates to create all possible sets of
    // evaluations at the river in omaha, this should produce (4c2)*(5c3) =
    // 6*10 = 60 candidates
    for (auto& hand_candidate : hand_candidates)
      for (const auto& board_candidate : board_candidates) {
        eval_candidates.push_back(hand_candidate | board_candidate);
      }

    // evaluation of the first type.  we do a quick evaluation
    // of the one candidate which *must* be there, and then if
    // there are more candidates, we just run through them updating
    // as we find better ones.
    eval[0] = ((eval_candidates[0]).*(m_evalA))();
    for (uint i = 1; i < eval_candidates.size(); i++) {
      PokerEvaluation e = ((eval_candidates[i]).*(m_evalA))();
      if (e > eval[0])
        eval[0] = e;
    }

    // if it's a one dimensional evaluation we are done
    if (m_evalB != evalFunction(nullptr)) {
      // second dimension of the evaulation, usually low in a high/low
      // game.
      eval[1] = PokerEvaluation();
      for (auto& eval_candidate : eval_candidates) {
        PokerEvaluation e = (eval_candidate.*(m_evalB))();
        if (e > eval[1]) {
          eval[1] = e;
        }
      }
    }
    return PokerHandEvaluation(eval[0], eval[1]);
  }

  void fillSubsets(std::vector<CardSet>& candidates, size_t subsetsize,
                   CardSet cards) const {
    if (subsetsize > cards.size()) {
      candidates.emplace_back();
      return;
    }
    if (subsetsize == 0) {
      candidates.push_back(cards);
      return;
    }

    std::vector<Card> clist = cards.cards();
    nit::combinations cc(static_cast<uint>(clist.size()),
                         static_cast<uint>(subsetsize));
    do {
      CardSet cand;
      for (size_t i = 0; i < subsetsize; i++) {
        cand.insert(clist[cc[static_cast<uint>(i)]]);
      }
      candidates.push_back(cand);
    } while (cc.nextcomb());
  }

  virtual size_t evalsPerHand() const { return m_evalsperhand; }

 private:
  size_t m_heromin;
  size_t m_heromax;
  size_t m_boardmin;
  size_t m_boardmax;
  size_t m_herouse;
  evalFunction m_evalA;
  evalFunction m_evalB;
  int m_evalsperhand;
};

}  // namespace nit

#endif  // NIT_PEVAL_UNIVERSAL_HAND_EVALUATOR_H_
