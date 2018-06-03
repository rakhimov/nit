/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#include "card_distribution.h"

#include <iostream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/math/special_functions/binomial.hpp>

#include <nit/peval/card.h>
#include <nit/util/combinations.h>

namespace nit {

CardDistribution::CardDistribution() : m_handList(1, CardSet()), m_weights() {
  m_weights[CardSet()] = 1.0;
}

CardDistribution::CardDistribution(const CardSet& cs)
    : m_handList(1, cs), m_weights() {
  m_weights[cs] = 1.0;
}

CardDistribution::CardDistribution(const CardDistribution& cd) { *this = cd; }

CardDistribution& CardDistribution::operator=(const CardDistribution& other) =
    default;

size_t CardDistribution::size() const { return m_handList.size(); }

std::string CardDistribution::str() const {
  std::string ret;
  for (size_t i = 0; i < m_handList.size(); i++) {
    const CardSet& hand = m_handList[i];
    ret += (i > 0 ? "," : "") +
           (boost::format("%s=%.3f") % hand.str() % weight(hand)).str();
  }
  return ret;
}

void CardDistribution::clear() {
  m_handList.clear();
  m_weights.clear();
}

CardDistribution CardDistribution::data() const { return *this; }

void CardDistribution::fill(int n) {
  CardSet cards;
  cards.fill();
  fill(cards, n);
}

void CardDistribution::fill(const CardSet& cs, int n) {
  std::vector<Card> cards = cs.cards();
  auto setsize = static_cast<int>(cards.size());
  combinations hands(setsize, n);
  int vsize = boost::math::binomial_coefficient<double>(setsize, n);
  clear();
  m_handList.reserve(vsize);

  do {
    CardSet cs;
    for (int i = 0; i < n; i++)
      cs.insert(cards[hands[i]]);
    m_handList.push_back(cs);
    m_weights[cs] = 1.0;
  } while (hands.next());
}

const CardSet& CardDistribution::operator[](size_t index) const {
  if (index >= m_handList.size())
    throw std::runtime_error("CardDistribution::operator: bounds error");
  return m_handList[index];
}

const double& CardDistribution::operator[](const CardSet& hand) const {
  static const double kStaticZero = 0.0;  // for hands not in distribution
  auto it = m_weights.find(hand);
  if (it == m_weights.end())
    return kStaticZero;
  return it->second;
}

double& CardDistribution::operator[](const CardSet& hand) {
  return m_weights[hand];
}

// this is a simple wrapper to make reading the code easier
const double& CardDistribution::weight(const CardSet& hand) const {
  return (*this)[hand];
}

bool CardDistribution::parse(const std::string& input) {
  clear();

  // trap the special "unrealized/empty" distribution which
  // has no specific cards, and is equivalent to a "random" dist
  // TODO: verify that this is the internal representation we want for a
  // random hand
  if (input == ".") {
    CardSet hand;
    if (hand.size() != 0)
      return false;
    m_weights[hand] = 1.0;
    m_handList.push_back(hand);
    return true;
  }

  std::vector<std::string> hands;
  boost::split(hands, input, boost::is_any_of(","));
  for (const std::string& h : hands) {
    // handle the weight
    double weight = 1.0;
    if (boost::contains(h, "=")) {
      // trap for the case where the input ends with "="
      std::string::size_type weightPos = h.rfind("=") + 1;
      if (weightPos == h.size())
        return false;
      weight = boost::lexical_cast<double>(h.substr(weightPos));
    }

    // handle the hand one card at a time.
    std::string handstr = h.substr(0, h.rfind("="));
    if (handstr.size() % 2 != 0)
      return false;
    CardSet hand;
    for (size_t i = 0; i < handstr.size(); i += 2) {
      Card c;
      if (!c.fromString(handstr.substr(i, i + 2)))
        return false;
      if (hand.contains(c))
        return false;
      hand.insert(c);
    }

    // final check and
    if (hand.size() == 0)
      return false;
    m_weights[hand] = weight;
    m_handList.push_back(hand);
  }
  return true;
}

std::string CardDistribution::display() const { return str(); }

void CardDistribution::removeCards(const CardSet& dead) {
  for (auto& i : m_handList)
    if (i.intersects(dead))
      m_weights[i] = 0.0;
}

double CardDistribution::weight() const {
  double total = 0.0;
  for (const std::pair<CardSet, double>& w : m_weights)
    total += w.second;
  return total;
}

}  // namespace nit
