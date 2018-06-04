/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#include "poker_hand.h"

#include <cassert>

#include <algorithm>

#include <nit/error.h>

#include "card_set.h"
#include "holdem.h"
#include "poker_evaluation.h"
#include "poker_evaluation_tables.h"

namespace nit {

PokerHand::PokerHand() { clear(); }

CardSet PokerHand::cardSet() const {
  CardSet cs;
  for (uint8_t i = 0; i < m_ncards; i++)
    cs.insert(m_cards[i]);
  return cs;
}

CardSet PokerHand::cardSet(size_t first, size_t len) const {
  CardSet cs;
  size_t last = std::min(first + len, static_cast<size_t>(m_ncards));
  for (size_t i = first; i < last; i++)
    cs.insert(m_cards[i]);
  return cs;
}

PokerHand::PokerHand(const CardSet& s) {
  clear();
  append(s);
}

PokerHand::PokerHand(const std::string& s) {
  clear();
  fromString(s);
}

void PokerHand::clear() { m_ncards = 0; }

// if we have few enough cards, we'll spit it out in order, otherwise
// just output them as a sorted set.
std::string PokerHand::str() const {
  std::string out = "";
  for (int i = 0; i < m_ncards; i++)
    out += m_cards[i].str();
  return out;
}

std::string PokerHand::preflopstr() const {
  if (m_ncards != 2)
    throw LogicError("incorrect number of cards for hold'em preflop canon");
  Card c0 = m_cards[0];
  Card c1 = m_cards[1];

  if (c0.rank() == c1.rank())
    return c0.rank().str() + c0.rank().str();

  if (c0.rank() < c1.rank())
    std::swap(c0, c1);

  if (c0.suit() == c1.suit())
    return c0.rank().str() + c1.rank().str() + "s";
  return c0.rank().str() + c1.rank().str() + "o";
}

void PokerHand::fromString(const std::string& instr) {
  clear();
  auto slen = instr.length();
  if (slen == 0)
    return;
  if (slen == 1)
    return;

  for (size_t i = 0; i < slen - 1; i++) {
    Card c;
    if (c.fromString(instr.substr(i))) {
      append(c);
      i++;
    }
  }
}

size_t PokerHand::size() const { return static_cast<size_t>(m_ncards); }

bool PokerHand::contains(const Card& c) const {
  for (uint8_t i = 0; i < m_ncards; i++)
    if (m_cards[i] == c)
      return true;
  return false;
}

void PokerHand::append(const Card& c) {
  if (contains(c))
    return;

  if (m_ncards < MAX_PHCARDS)
    m_cards[m_ncards++] = c;
}

void PokerHand::append(const CardSet& cs) {
  std::vector<Card> cards = cs.cards();
  for (auto card : cards)
    append(card);
}

void PokerHand::append(const PokerHand& h) {
  for (int i = 0; i < h.m_ncards; i++)
    append(h.m_cards[i]);
}

void PokerHand::remove(const Card& c) {
  for (uint8_t i = 0, j = 0; i < m_ncards; i++, j++) {
    if (c == m_cards[i]) {
      j++;
      m_ncards--;
    }
    m_cards[i] = m_cards[j];
  }
}

Card& PokerHand::operator[](size_t index) { return m_cards[index]; }

const Card& PokerHand::operator[](size_t index) const { return m_cards[index]; }

void PokerHand::sort() const {
  std::sort(m_cards.begin(), m_cards.begin() + m_ncards);
}

void PokerHand::pushCardToFront(size_t nth) const {
  if (nth >= m_ncards)
    return;
  Card c = m_cards[nth];
  for (size_t i = nth; i > 0; i--)
    m_cards[i] = m_cards[i - 1];
  m_cards[0] = c;
}

static bool rankgreater(Card c1, Card c2) {
  if (c1.rank() > c2.rank())
    return true;
  return false;
}

void PokerHand::sortRanks() const {
  std::sort(m_cards.begin(), m_cards.begin() + m_ncards, rankgreater);
}

void PokerHand::sortEval() const {
  PokerHand oldh = *this;
  PokerHand newh;
  PokerEvaluation eval = cardSet().evaluateHigh();
  Rank r;
  bool dominor = true;
  Rank straighttop = Rank::Five();

  oldh.sortRanks();
  switch (eval.type()) {
      // these types have major only
    case ONE_PAIR:
    case THREE_OF_A_KIND:
    case FOUR_OF_A_KIND:
      dominor = false;

      // these have major and minor
    case TWO_PAIR:
    case FULL_HOUSE:
      r = eval.majorRank();
      while (oldh.cardSet().contains(r)) {
        Card c = oldh.cardSet().find(r);
        newh.append(c);
        oldh.remove(c);
      }
      if (dominor) {
        r = eval.minorRank();
        while (oldh.cardSet().contains(r)) {
          Card c = oldh.cardSet().find(r);
          newh.append(c);
          oldh.remove(c);
        }
      }
      // we pass through here, no break

    case NO_PAIR:
    case THREE_FLUSH:
    case THREE_STRAIGHT_FLUSH:
    case FLUSH:
      newh.append(oldh);
      break;

      // straights may require the ace to swing low
    case THREE_STRAIGHT:
      straighttop = Rank::Three();
    case STRAIGHT:
    case STRAIGHT_FLUSH:
      newh = oldh;
      r = eval.majorRank();
      if (r == straighttop) {
        Card c = newh.cardSet().find(Rank::Ace());
        newh.remove(c);
        newh.append(c);
      }
      break;
  }

  for (int i = 0; i < m_ncards; i++)
    m_cards[i] = newh.m_cards[i];
}

void PokerHand::remove(const CardSet& cs) {
  std::vector<Card> cards = cs.cards();
  for (auto card : cards)
    remove(card);
}

void PokerHand::remove(const PokerHand& ph) {
  for (uint8_t i = 0; i < ph.m_ncards; i++)
    remove(ph.m_cards[i]);
}

std::vector<Card> PokerHand::cards() const {
  std::vector<Card> out;
  for (uint8_t i = 0; i < m_ncards; i++)
    out.push_back(m_cards[i]);
  return out;
}

}  // namespace nit
