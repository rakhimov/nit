#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include <boost/program_options.hpp>

#include <nit/eval/card.h>
#include <nit/eval/card_set.h>
#include <nit/util/combinations.h>

#include "guard.h"

namespace po = boost::program_options;

namespace {

int runColex(int argc, char** argv) {
  // set up the program options, handle the help case, and extract the values.
  po::options_description desc(
      "nit-colex, a utility which prints all combinations "
      "of poker hands, using canonical suits, or only ranks");

  // clang-format off
  desc.add_options()
      ("help,?", "produce help message")
      ("num-cards,n", po::value<std::size_t>()->default_value(2),
       "number of cards in hands")
      ("ranks", "print the set of rank values");
  // clang-format on

  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
  } catch (const std::exception& err) {
    std::cerr << "Option error: " << err.what() << "\n\n" << desc << std::endl;
    return 1;
  }
  po::notify(vm);

  // check for help
  if (vm.count("help") || argc == 1) {
    std::cout << desc << std::endl;
    return 1;
  }

  // extract the options
  std::size_t num_cards = vm["num-cards"].as<std::size_t>();

  std::set<nit::CardSet> canonicalHands;
  std::map<std::string, std::size_t> rankHands;
  nit::combinations cards(52, num_cards);
  do {
    nit::CardSet hand;
    for (std::size_t i = 0; i < num_cards; i++) {
      hand.insert(nit::Card(cards[i]));
    }
    canonicalHands.insert(hand.canonize());
    rankHands[hand.rankstr()] = hand.rankColex();
  } while (cards.next());

  if (vm.count("ranks") > 0) {
    for (auto& rankHand : rankHands)
      std::cout << rankHand.first << ": " << rankHand.second << '\n';
  } else {
    for (const auto& canonicalHand : canonicalHands) {
      std::cout << canonicalHand.str() << ": " << canonicalHand.colex() << '\n';
    }
  }

  return 0;
}

}  // namespace

int main(int argc, char** argv) {
  return guard([&argc, &argv] { return runColex(argc, argv); });
}
