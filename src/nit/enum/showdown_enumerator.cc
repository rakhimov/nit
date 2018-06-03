/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#include "showdown_enumerator.h"

#include <vector>

#include <nit/error.h>

#include "odometer.h"
#include "partition_enumerator.h"
#include "simple_deck.h"

namespace nit {

ShowdownEnumerator::ShowdownEnumerator() = default;

std::vector<EquityResult> ShowdownEnumerator::calculateEquity(
    const std::vector<CardDistribution>& dists, const CardSet& board,
    std::shared_ptr<PokerHandEvaluator> peval) const {
  if (peval.get() == nullptr)
    throw LogicError("ShowdownEnumerator, null evaluator");
  assert(dists.size() > 1);
  const size_t ndists = dists.size();
  std::vector<EquityResult> results(ndists, EquityResult());
  size_t handsize = peval->handSize();

  // the dsizes vector is a list of the sizes of the player hand
  // distributions
  std::vector<size_t> dsizes;
  for (size_t i = 0; i < ndists; i++) {
    assert(dists[i].size() > 0);
    dsizes.push_back(dists[i].size());
  }

  // need to figure out the board stuff, we'll be rolling the board into
  // the partitions to make enumeration easier down the line.
  size_t nboards = 0;
  size_t boardsize = peval->boardSize();
  if (boardsize > 0)
    nboards++;

  // for the most part, these are allocated here to avoid contant stack
  // reallocation as we cycle through the inner loops
  SimpleDeck deck;
  CardSet dead;
  double weight;
  std::vector<CardSet> ehands(ndists + nboards);
  std::vector<size_t> parts(ndists + nboards);
  std::vector<CardSet> cardPartitions(ndists + nboards);
  std::vector<PokerHandEvaluation> evals(ndists);  // NO BOARD

  // copy quickness
  CardSet* copydest = &ehands[0];
  CardSet* copysrc = &cardPartitions[0];
  size_t ncopy = (ndists + nboards) * sizeof(CardSet);
  Odometer o(dsizes);
  do {
    // colect all the cards being used by the players, skip out in the
    // case of card duplication
    bool disjoint = true;
    dead.clear();
    weight = 1.0;
    for (size_t i = 0; i < ndists + nboards; i++) {
      if (i < ndists) {
        cardPartitions[i] = dists[i][o[i]];
        parts[i] = handsize - cardPartitions[i].size();
        weight *= dists[i][cardPartitions[i]];
      } else {
        // this allows us to have board distributions in the future
        cardPartitions[i] = board;
        parts[i] = boardsize - cardPartitions[i].size();
      }
      disjoint = disjoint && dead.disjoint(cardPartitions[i]);
      dead |= cardPartitions[i];
    }

    if (disjoint) {
      deck.reset();
      deck.remove(dead);
      PartitionEnumerator2 pe(deck.size(), parts);
      do {
        // we use memcpy here for a little speed bonus
        memcpy(copydest, copysrc, ncopy);
        for (size_t p = 0; p < ndists + nboards; p++)
          ehands[p] |= deck.peek(pe.getMask(p));

        // TODO: do we need this if/else, or can we just use the if
        // clause? A: need to rework tracking of whether a board is
        // needed
        if (nboards > 0)
          peval->evaluateShowdown(ehands, ehands[ndists], evals, results,
                                  weight);
        else
          peval->evaluateShowdown(ehands, board, evals, results, weight);
      } while (pe.next());
    }
  } while (o.next());

  return results;
}

}  // namespace nit
