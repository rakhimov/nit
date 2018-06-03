#include <exception>
#include <iostream>
#include <set>
#include <string>

#include <boost/format.hpp>
#include <boost/program_options.hpp>

#include <nit/eval/card.h>
#include <nit/eval/card_set.h>
#include <nit/eval/card_set_generators.h>
#include <nit/eval/poker_hand_evaluator.h>
#include <nit/util/combinations.h>

#include "guard.h"

namespace po = boost::program_options;

namespace {

int runLut(int argc, char** argv) {
  // set up the program options,
  // handle the help case, and extract the values
  po::options_description desc("creates a lookup table for poker hands");
  // clang-format off
  desc.add_options()
      ("help,?", "produce help message")
      ("pocket-count,p", po::value<size_t>()->default_value(2),
       "number of pocket cards to use")
      ("board-count,b", po::value<size_t>()->default_value(3),
       "number of board cards to use")
      ("game,g", po::value<std::string>()->default_value("O"),
       "game to use for evaluation")
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
  size_t pocketCount = vm["pocket-count"].as<size_t>();
  size_t boardCount = vm["board-count"].as<size_t>();
  std::string game = vm["game"].as<std::string>();

  bool ranks = vm.count("ranks") > 0;
  // make the sets
  nit::Card::Grouping grouping = nit::Card::SUIT_CANONICAL;
  if (ranks)
    grouping = nit::Card::RANK;
  std::set<nit::CardSet> pockets = nit::createCardSet(pocketCount, grouping);
  std::set<nit::CardSet> boards = nit::createCardSet(boardCount, grouping);

  auto evaluator = nit::PokerHandEvaluator::alloc(game);
  for (const auto& pocket : pockets)
    for (const auto& board : boards) {
      if (ranks) {
        nit::PokerEvaluation eval = evaluator->evaluateRanks(pocket, board);
        // cout << boost::format("%s, %s\n") % pit->rankstr() %
        // bit->rankstr();

        // clang-format off
        std::cout << boost::format("%s, %s: [%4d,%4d] -> %s [%9d]\n") %
                         pocket.str() %
                         board.str() %
                         pocket.rankColex() %
                         board.rankColex() %
                         eval.str() %
                         eval.code();
        // clang-format on
      } else {
        board.canonize(pocket);
        if (pocket.intersects(board))
          continue;
        nit::PokerHandEvaluation eval = evaluator->evaluate(pocket, board);
        // clang-format off
        std::cout << boost::format("%s, %s: [%4d,%4d] -> %s [%9d]\n") %
                         pocket.str() %
                         board.str() %
                         pocket.colex() %
                         board.colex() %
                         eval.str() %
                         eval.high().code();
        // clang-format on
      }
    }

  return 0;
}

}  // namespace

int main(int argc, char** argv) {
  return guard([&argc, &argv] { return runLut(argc, argv); });
}
