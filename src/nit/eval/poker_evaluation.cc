/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#include "poker_evaluation.h"

#include <iostream>

#include <boost/format.hpp>

#include <nit/util/lastbit.h>

#include "card.h"
#include "poker_evaluation_tables.h"

namespace nit {

// some useful consts that don't need to be exposd
const int MAJOR_MASK = 0xF << MAJOR_SHIFT;
const int MINOR_MASK = 0xF << MINOR_SHIFT;
const int KICKER_MASK = 0x1FFF;

PokerEvaluation::PokerEvaluation() = default;

PokerEvaluation::PokerEvaluation(int ecode) : m_evalcode(ecode) {}

int PokerEvaluation::code() const { return m_evalcode; }

int PokerEvaluation::reducedCode() const {
  if (isFlipped()) {
    PokerEvaluation e = *this;
    e.flip();
    return e.reducedCode2to7();
  }

  if (m_evalcode == 0)
    return 0;
  switch (type()) {
    case NO_PAIR:
    case ONE_PAIR:
    case THREE_FLUSH:
    case FLUSH: {
      PokerEvaluation e(m_evalcode);
      int kick1 = topRankTable[kickerBits()];
      int kick2 = topRankTable[kickerBits() ^ 0x01 << kick1];
      int kbits = (0x01 << kick1) | (0x01 << kick2);
      e.setKickerBits(kbits);
      return e.code();
    }

    case FULL_HOUSE:
    case THREE_OF_A_KIND:
    case FOUR_OF_A_KIND:
      return showdownCode();

    default:
      return code();
  }
}

int PokerEvaluation::reducedCode2to7() const {
  if (m_evalcode == 0)
    return 0;
  switch (type()) {
    case NO_PAIR: {
      PokerEvaluation e(m_evalcode);
      int kick1 = topRankTable[kickerBits()];
      int kick2 = topRankTable[kickerBits() ^ 0x01 << kick1];
      int kick3 = topRankTable[kickerBits() ^ 0x01 << kick1 ^ 0x01 << kick2];
      int kbits = (0x01 << kick1) | (0x01 << kick2);
      if (kick1 < 10)
        kbits |= (0x01 << kick3);
      e.setKickerBits(kbits);
      e.flip();
      return e.code();
    }

    case THREE_OF_A_KIND: {
      PokerEvaluation e(m_evalcode);
      std::vector<int> ranks(3);
      ranks[0] = botRankTable[kickerBits()];
      ranks[1] = botRankTable[kickerBits() ^ 0x01 << ranks[0]];
      ranks[2] = e.majorRank().code();
      sort(ranks.begin(), ranks.end());
      e = PokerEvaluation((TWO_PAIR << VSHIFT) ^ (ranks[2] << MAJOR_SHIFT) ^
                          (ranks[1] << MINOR_SHIFT) ^ (0x01 << ranks[0]));
      e.flip();
      return e.code();
    }

    case ONE_PAIR: {
      PokerEvaluation e(m_evalcode);
      std::vector<int> ranks(4);
      uint16_t kickers = kickerBits();
      ranks[0] = lastbit(kickers);
      kickers ^= 0x01 << ranks[0];
      ranks[1] = lastbit(kickers);
      kickers ^= 0x01 << ranks[1];
      ranks[2] = lastbit(kickers);
      kickers ^= 0x01 << ranks[2];
      ranks[3] = e.majorRank().code();
      sort(ranks.begin(), ranks.end());
      e = PokerEvaluation((ONE_PAIR << VSHIFT) ^ (ranks[2] << MAJOR_SHIFT) ^
                          (0x01 << ranks[0]) ^ (0x01 << ranks[1]));
      e.flip();
      return e.code();
    }

    case TWO_PAIR: {
      PokerEvaluation e(m_evalcode);
      std::vector<int> ranks(3);
      uint16_t kickers = kickerBits();
      ranks[0] = lastbit(kickers);
      ranks[1] = e.minorRank().code();
      ranks[2] = e.majorRank().code();
      sort(ranks.begin(), ranks.end());
      e = PokerEvaluation((TWO_PAIR << VSHIFT) ^ (ranks[2] << MAJOR_SHIFT) ^
                          (ranks[1] << MINOR_SHIFT) ^ (0x01 << ranks[0]));
      e.flip();
      return e.code();
    }

    case FLUSH: {
      PokerEvaluation e(m_evalcode);
      int kick1 = topRankTable[kickerBits()];
      int kbits = (0x01 << kick1);
      e.setKickerBits(kbits);
      e.flip();
      return e.code();
    }

    case FULL_HOUSE: {
      PokerEvaluation e(m_evalcode);
      std::vector<int> ranks(2);
      ranks[0] = e.minorRank().code();
      ranks[1] = e.majorRank().code();
      sort(ranks.begin(), ranks.end());
      e = PokerEvaluation((FULL_HOUSE << VSHIFT) ^ (ranks[1] << MAJOR_SHIFT) ^
                          (ranks[0] << MINOR_SHIFT));
      e.flip();
      return e.code();
    }

    case FOUR_OF_A_KIND: {
      PokerEvaluation e(m_evalcode);
      std::vector<int> ranks(2);
      uint16_t kickers = kickerBits();
      ranks[0] = lastbit(kickers);
      ranks[1] = e.majorRank().code();
      e = PokerEvaluation((FOUR_OF_A_KIND << VSHIFT) ^
                          (ranks[1] << MAJOR_SHIFT) ^ (0x01 << ranks[0]));
      e.flip();
      return e.code();
    }

    default:
      return code();
  }
}

int PokerEvaluation::showdownCode() const {
  if (m_evalcode == 0)
    return 0;

  int minor = 0;
  switch (type()) {
    case FULL_HOUSE:  // fall through case, prints deuces full nicer
      if (majorRank() == Rank::Two())
        minor = 1;
    case THREE_OF_A_KIND:
    case FOUR_OF_A_KIND: {
      PokerEvaluation e(m_evalcode);
      e.setMinorRank(minor);
      e.setKickerBits(0);
      return e.code();
    }

    default:
      return code();
  }
}

int PokerEvaluation::type() const { return m_evalcode >> VSHIFT; }

int PokerEvaluation::kickerBits() const { return m_evalcode & KICKER_MASK; }
Rank PokerEvaluation::majorRank() const {
  return Rank((m_evalcode >> MAJOR_SHIFT) & 0x0F);
}
Rank PokerEvaluation::minorRank() const {
  return Rank((m_evalcode >> MINOR_SHIFT) & 0x0F);
}
void PokerEvaluation::setKickerBits(int k) {
  m_evalcode = (m_evalcode & ~KICKER_MASK) | k;
}
void PokerEvaluation::setMajorRank(int r) {
  m_evalcode = (m_evalcode & ~MAJOR_MASK) | r << MAJOR_SHIFT;
}
void PokerEvaluation::setMinorRank(int r) {
  m_evalcode = (m_evalcode & ~MINOR_MASK) | r << MINOR_SHIFT;
}

// this converts the evaluation to one where
// the ace is valued as low instead of high,
// thus 2 > A internally.
void PokerEvaluation::playAceLow() {
  if (acePlaysLow())
    return;

  m_evalcode |= ACE_LOW_BIT;

  // make room for the ace in kicker land, and if
  // we have an ace, push it to the bottom
  int kickers = kickerBits();
  kickers = kickers << 1;
  if (kickers & ACE_LOW_BIT)
    kickers++;
  setKickerBits(kickers);

  switch (type()) {
    case TWO_PAIR:
    case FULL_HOUSE:
      setMinorRank((minorRank().code() + 1) % Rank::NUM_RANK);

    case ONE_PAIR:
    case THREE_OF_A_KIND:
    case STRAIGHT:
    case FOUR_OF_A_KIND:
    case STRAIGHT_FLUSH:
      setMajorRank((majorRank().code() + 1) % Rank::NUM_RANK);
      break;
  };
}

void PokerEvaluation::playAceHigh() {
  if (acePlaysLow() == false)
    return;

  m_evalcode ^= ACE_LOW_BIT;

  // make room for the ace in kicker land, and if
  // we have an ace, push it to the bottom
  int kickers = kickerBits();
  if (kickers & 0x01)
    kickers |= ACE_LOW_BIT;
  kickers = kickers >> 1;
  setKickerBits(kickers);
}

bool PokerEvaluation::acePlaysLow() const {
  if (m_evalcode & ACE_LOW_BIT)
    return true;
  return false;
}

void PokerEvaluation::fixWheel2to7(int rankMask) {
  if (type() == STRAIGHT && majorRank() == Rank::Five()) {
    m_evalcode = (NO_PAIR << VSHIFT) ^ rankMask;
  } else if (type() == STRAIGHT_FLUSH && majorRank() == Rank::Five()) {
    m_evalcode = (FLUSH << VSHIFT) ^ rankMask;
  }
}

std::string PokerEvaluation::makeRankString(int r, bool acelow) const {
  if (acelow) {
    if (r == 0)
      r = Rank::NUM_RANK - 1;
    else
      r--;
  }

  Rank rank(r);
  return rank.str();
}

std::string PokerEvaluation::strKickers(int n) const {
  std::string out;
  for (int i = Rank::NUM_RANK - 1; i >= 0; i--)
    if ((n & (0x01 << i)) > 0) {
      //      Rank r(i);
      //      out += r.toString ();
      out += makeRankString(i, acePlaysLow());
    }
  return out;
}

std::string PokerEvaluation::strTop(int n) const {
  int i = (n >> 20) & 0x0F;
  //    Rank r(i);
  //    return r.toString ();
  return makeRankString(i, acePlaysLow());
}

std::string PokerEvaluation::strBot(int n) const {
  int i = (n >> 16) & 0x0F;
  //    Rank r(i);
  //    return r.toString ();
  return makeRankString(i, acePlaysLow());
}

std::string PokerEvaluation::bitstr() const {
  std::string ret;
  int n = m_evalcode;
  for (int i = 31; i >= 0; i--) {
    if ((n & (0x01 << i)) > 0)
      ret += "1";
    else
      ret += "0";
    if (i % 8 == 0)
      ret += " ";
  }
  return ret;
}

std::string PokerEvaluation::str() const {
  std::string ret;
  int n = m_evalcode;

  if (n == 0)
    return "";

  if (n != 0) {
    if (isFlipped()) {
      PokerEvaluation e(m_evalcode);
      e.flip();
      return e.str();
    }
    // ret += " ";
    ret += toStringCannon();
  }
  return ret;
}

std::string PokerEvaluation::toStringCannon() const {
  const std::string highcard = "high card:    ";
  const std::string onepair = "one pair:     ";
  const std::string threeflush = "three flush:  ";
  const std::string threestraight = "three str8:   ";
  const std::string twopair = "two pair:     ";
  const std::string trips = "trips:        ";
  const std::string threestr8flush = "3 str8 flush: ";
  const std::string straight = "straight:     ";
  const std::string flush = "flush:        ";
  const std::string fullhouse = "full house:   ";
  const std::string quads = "quads:        ";
  const std::string straightflush = "str8 flush:   ";

  std::string ret;
  int n = m_evalcode;

  if (isFlipped()) {
    PokerEvaluation e(m_evalcode);
    e.flip();
    return e.toStringCannon();
  }

  std::string hand;
  std::string topr;
  std::string botr;
  std::string kick;

  int val = n >> 24;
  switch (val) {
    case NO_PAIR:
      hand += highcard;
      kick += strKickers(n);
      break;

    case ONE_PAIR:
      hand += onepair;
      topr += strTop(n);
      kick += strKickers(n);
      break;

    case THREE_FLUSH:
      hand += threeflush;
      kick += strKickers(n);
      break;

    case THREE_STRAIGHT:
      hand += threestraight;
      topr += strTop(n);
      break;

    case TWO_PAIR:
      hand += twopair;
      topr += strTop(n);
      botr += strBot(n);
      kick += strKickers(n);
      break;

    case THREE_OF_A_KIND:
      hand += trips;
      topr += strTop(n);
      kick += strKickers(n);
      break;

    case THREE_STRAIGHT_FLUSH:
      hand += threestr8flush;
      topr += strTop(n);
      break;

    case STRAIGHT:
      hand += straight;
      topr += strTop(n);
      break;

    case FLUSH:
      hand += flush;
      topr += strKickers(n);
      break;

    case FULL_HOUSE:
      hand += fullhouse;
      topr += strTop(n);
      botr += strBot(n);
      break;

    case FOUR_OF_A_KIND:
      hand += quads;
      topr += strTop(n);
      botr += strKickers(n);
      break;

    case STRAIGHT_FLUSH:
      hand += straightflush;
      kick += strTop(n);
      break;
  }

  std::string ranks = topr + botr + kick;
  ret = (boost::format("%s %-5s") % hand % ranks).str();

  return ret;
}

void PokerEvaluation::generateLowballLookupA5() {
  std::cout << "const int lowballA5Ranks[] = {\n";
  // this is a 13 bit lookup table
  for (int i = 0; i <= 0x1FFF; i++) {
    // ok, we are passing in plain old rankbits, but
    // geting A-5 lowball bits out, let's convert first off
    int bits = i << 1;
    if (bits & ACE_LOW_BIT)
      bits = bits - ACE_LOW_BIT + 1;

    int lowbits = 0;
    int nbits = 0;
    for (int b = 0; b < 13; b++) {
      if (bits & 0x01 << b) {
        lowbits += 0x01 << b;
        nbits++;
      }
      if (nbits == 5)
        break;
    }
    std::cout << boost::format("    %6d, // %s\n") % lowbits %
                     toBitString(lowbits);
  }
  std::cout << "};\n";
}

void PokerEvaluation::generateBottomRankMask() {
  std::cout << "const int bottomRankMask[] = {\n";
  // this is a 13 bit lookup table
  for (int i = 0; i <= 0x1FFF; i++) {
    int bits = i;
    int lowbits = 0;
    for (int b = 0; b < 13; b++) {
      if (bits & 0x01 << b) {
        lowbits += 0x01 << b;
        break;
      }
    }
    std::cout << boost::format("    %6d, // %s\n") % lowbits %
                     toBitString(lowbits);
  }
  std::cout << "};\n";
}

}  // namespace nit
