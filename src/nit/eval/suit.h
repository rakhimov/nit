/**
 * Copyright (c) 2012 Andrew Prock. All rights reserved.
 */
#ifndef NIT_EVAL_SUIT_H_
#define NIT_EVAL_SUIT_H_

#include <cstdint>

#include <string>

namespace nit {

/**
 * A suit class for cards.  This class encodes the standard four
 * suit deck of {clubs,diamonds,hears,spades}.  The bridge ordering
 * of suits is used for determining relative order of suits when
 * needed.
 */
class Suit {
 public:
  // class related constants
  static const unsigned NUM_SUIT = 4;
  static const Suit Clubs() { return Suit(0); }
  static const Suit Diamonds() { return Suit(1); }
  static const Suit Hearts() { return Suit(2); }
  static const Suit Spades() { return Suit(3); }

  // for iteration
  static const Suit begin() { return Suit(0); }
  static const Suit end() { return Suit(NUM_SUIT); }

  // standalone fast parsting
  static int suit_code(char c);

  /**
   * Default constructor initializes suit to Suit::Clubs()
   */
  Suit() = default;

  /**
   * Create from input string.  Only the first letter of the input
   * string is examined, and must be in [cCdDhHsS] for a valid Suit
   * object to be created.
   */
  explicit Suit(const std::string& str);
  explicit Suit(uint8_t c);

  /**
   * Encode suit as a string.  There are sevral encodings described
   * in the display enum.
   * @see Suit::display
   * @see Suit::setSuitStringType()
   */
  std::string str() const;

  enum display {
    SUIT_ASCII = 1,       //!< plain old ascii chars
    SUIT_ASCII_EXTENDED,  //!< uses code page 437 to display suit symbols
    SUIT_HTML,            //!< html suit symbols
    SUIT_HTML_2COLOR,     //!<   with red/black colors
    SUIT_HTML_4COLOR,     //!<     or red/black/green/blue colors
    SUIT_PREFLOP_CANNON,  //!< broken?
    SUIT_ANSI_EXT_COLOR,  //!< code page 437 with ANSI color
    SUIT_UNICODE
  };  //!< ANSI color with ascii chars

  /**
   * set how suits are displayed.  the type of display that is used
   * is a global state variable and can be changed to suit the
   * output being generated.
   */
  static void setSuitStringType(Suit::display s);

  /**
   * query suit display state.  used for save/restore of suit state
   * if needed
   */
  static display getSuitStringType();

  /**
   * Construct suit from the first char of the input string.  Exception will
   * be thrown if the character is not a valid suit character.
   */
  bool operator==(const Suit& s) const { return m_suit == s.m_suit; }
  bool operator<=(const Suit& s) const { return m_suit <= s.m_suit; }
  bool operator<(const Suit& s) const { return m_suit < s.m_suit; }
  void operator++() { ++m_suit; }
  void operator--() { --m_suit; }

 private:
  /**
   * We break encapsulation with these methods.  To limit the
   * effects of leakage, we limit access to these functions to
   * friends.
   */
  uint8_t code() const { return m_suit; }
  void encode(uint8_t c) { m_suit = c; }
  int suitBit() const { return 0x01 << m_suit * 3; }

  void fromString(const std::string& s);

  static bool isSuitChar(char c);

  friend class Card;
  friend class CardSet;

  uint8_t m_suit{0};

  static display m_suitStringType;

  std::string decodeSuitASCII(int c) const;
  std::string decodeSuitASCII_EXT(int card) const;
  std::string decodeSuitHTML(int c) const;
  std::string decodeSuit4ColorHTML(int c) const;
  std::string decodeSuit2ColorHTML(int c) const;
  std::string decodeSuit(int c) const;
  std::string decodeSuitANSI_EXT(int c) const;
  std::string decodeSuitUnicode(int c) const;

  static uint8_t ClubVal() { return 0; }
  static uint8_t DiamondVal() { return 1; }
  static uint8_t HeartVal() { return 2; }
  static uint8_t SpadeVal() { return 3; }
};

}  // namespace nit

#endif  // NIT_EVAL_SUIT_H_
