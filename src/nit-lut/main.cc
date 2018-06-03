#include <iostream>
#include <set>
#include <string>

#include <boost/format.hpp>
#include <boost/program_options.hpp>

#include <nit/peval/card.h>
#include <nit/peval/card_set.h>
#include <nit/peval/card_set_generators.h>
#include <nit/peval/poker_hand_evaluator.h>
#include <nit/util/combinations.h>

namespace po = boost::program_options;

int main(int argc, char** argv) {
  try {
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
    po::store(po::command_line_parser(argc, argv)
                  .style(po::command_line_style::unix_style)
                  .options(desc)
                  .run(),
              vm);
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
    for (auto pit = pockets.begin(); pit != pockets.end(); pit++)
      for (auto bit = boards.begin(); bit != boards.end(); bit++) {
        if (ranks) {
          nit::PokerEvaluation eval = evaluator->evaluateRanks(*pit, *bit);
          // cout << boost::format("%s, %s\n") % pit->rankstr() %
          // bit->rankstr();

          // clang-format off
          std::cout << boost::format("%s, %s: [%4d,%4d] -> %s [%9d]\n") %
                           pit->str() %
                           bit->str() %
                           pit->rankColex() %
                           bit->rankColex() %
                           eval.str() %
                           eval.code();
          // clang-format on
        } else {
          bit->canonize(*pit);
          if (pit->intersects(*bit))
            continue;
          nit::PokerHandEvaluation eval = evaluator->evaluate(*pit, *bit);
          // clang-format off
          std::cout << boost::format("%s, %s: [%4d,%4d] -> %s [%9d]\n") %
                           pit->str() %
                           bit->str() %
                           pit->colex() %
                           bit->colex() %
                           eval.str() %
                           eval.high().code();
          // clang-format on
        }
      }
  } catch (std::exception& e) {
    std::cerr << "-- caught exception--\n" << e.what() << "\n";
    return 1;
  } catch (...) {
    std::cerr << "Exception of unknown type!\n";
    return 1;
  }
  return 0;
}
