/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#ifndef NIT_PENUM_SIMPLE_DECK_H_
#define NIT_PENUM_SIMPLE_DECK_H_

#include <algorithm>
#include <array>
#include <functional>
#include <random>
#include <string>

#include <nit/peval/card.h>
#include <nit/peval/card_set.h>
#include <nit/peval/rank.h>  // NUM_RANK
#include <nit/peval/suit.h>  // NUM_SUIT
#include <nit/util/lastbit.h>

namespace nit {

/**
 * A very simple deck of the cards.
 */
class SimpleDeck {
 public:
  /**
   * construct a deck that is in-order
   */
  SimpleDeck() {
    for (uint8_t i = 0; i < STANDARD_DECK_SIZE; i++) {
      m_deck[i] = CardSet(Card(i));
    }
    reset();
  }

  /**
   * put all dealt cards back into deck, don't reorder
   */
  void reset() { m_current = STANDARD_DECK_SIZE; }

  /**
   * number of cards left in the deck
   */
  size_t size() const { return m_current; }

  /**
   * print cards in deck with un/dealt divider
   */
  std::string str() const {
    std::string ret;
    for (uint i = 0; i < STANDARD_DECK_SIZE; i++) {
      if (i == m_current)
        ret += "/";
      ret = ret + m_deck[i].str();
    }
    if (m_current == STANDARD_DECK_SIZE)
      ret += "/";
    return ret;
  }

  nit::CardSet deal(size_t ncards) {
    // TODO: fix and test this code, edge cases clearly at risk here
    if (ncards == 0)
      return nit::CardSet();
    m_current -= static_cast<uint>(ncards);
    CardSet* pcur = &m_deck[m_current];
    const CardSet* pend = pcur + ncards;
    CardSet cards(*pcur++);
    while (pcur < pend)
      cards |= *pcur++;
    return cards;
  }

  nit::CardSet dead() const {
    nit::CardSet cs;
    for (size_t i = m_current; i < STANDARD_DECK_SIZE; i++)
      cs.insert(m_deck[i]);
    return cs;
  }

  /**
   * Move all cards which are not live to the end of the deck
   */
  void remove(const nit::CardSet& cards) {
    int decr = CardSet(cards | dead()).size();
    std::stable_partition(
        m_deck.begin(), m_deck.end(),
        [&cards](const CardSet& c) { return !cards.contains(c); });
    m_current = STANDARD_DECK_SIZE - decr;
  }

  /**
   * look at ith card from the top of the deck
   */
  CardSet operator[](size_t i) const { return m_deck[i]; }

  void shuffle() {
    std::shuffle(m_deck.begin(), m_deck.end(),
                 std::mt19937(std::random_device()()));
    reset();  //m_current = 0;
  }

  /**
   * peek at the set of cards defined by the mask
   */
  CardSet peek(uint64_t mask) const {
#ifdef WIN32
// disable the unary negation of unsigned int
#pragma warning(disable : 4146)
#endif
    CardSet ret;

    auto lower = static_cast<uint32_t>(mask & UINT32_C(0xFFFFFFFF));
    auto upper =
        static_cast<uint32_t>((mask & UINT64_C(0xFFFFFFFF00000000)) >> 32);

    while (lower) {
      ret |= m_deck[lastbit(lower)];
      lower ^= (lower & -lower);
    }
    const CardSet* top = &m_deck[32];
    while (upper) {
      ret |= top[lastbit(upper)];
      upper ^= (upper & -upper);
    }

#ifdef WIN32
// set back to default
#pragma warning(default : 4146)
#endif

    return ret;
  }

 private:
  // these are the data which track info about the deck
  std::array<CardSet, STANDARD_DECK_SIZE> m_deck;
  size_t m_current;
};

}  // namespace nit

#endif  // NIT_PENUM_SIMPLE_DECK_H_
