#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include <boost/program_options.hpp>

#include <nit/enum/showdown_enumerator.h>
#include <nit/eval/make_evaluator.h>

#include "guard.h"

namespace po = boost::program_options;

namespace {

int runEval(int argc, char** argv) {
  po::options_description desc("nit-eval, a poker hand evaluator");

  // clang-format off
  desc.add_options()
      ("help,?", "produce help message")
      ("game,g", po::value<std::string>()->default_value("h"),
       "game to use for evaluation")
      ("board,b", po::value<std::string>(), "community cards for he/o/o8")
      ("hand,h", po::value<std::vector<std::string>>(), "a hand for evaluation")
      ("quiet,q", "produces no output");
  // clang-format on

  // make hand a positional argument
  po::positional_options_description p;
  p.add("hand", -1);

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
  std::string game = vm["game"].as<std::string>();
  std::string board = vm.count("board") ? vm["board"].as<std::string>() : "";
  std::vector<std::string> hands = vm["hand"].as<std::vector<std::string>>();

  bool quiet = vm.count("quiet") > 0;

  // allocate evaluator and create card distributions
  std::unique_ptr<nit::PokerHandEvaluator> evaluator = nit::makeEvaluator(game);
  std::vector<nit::CardDistribution> handDists;
  for (const std::string& hand : hands) {
    handDists.emplace_back();
    handDists.back().parse(hand);
  }

  // fill with random if necessary
  if (handDists.size() == 1) {
    handDists.emplace_back();
    handDists.back().fill(evaluator->handSize());
  }

  // calculate the results and print them
  nit::ShowdownEnumerator showdown;
  std::vector<nit::EquityResult> results =
      showdown.calculateEquity(handDists, nit::CardSet(board), *evaluator);

  double total = 0.0;
  for (const nit::EquityResult& result : results) {
    total += result.winShares + result.tieShares;
  }

  if (!quiet) {
    for (size_t i = 0; i < results.size(); ++i) {
      double equity = (results[i].winShares + results[i].tieShares) / total;
      std::string handDesc =
          (i < hands.size()) ? "The hand " + hands[i] : "A random hand";
      std::cout << handDesc << " has " << equity * 100. << " % equity ("
                << results[i].str() << ")" << std::endl;
    }
  }

  return 0;
}

}  // namespace

int main(int argc, char** argv) {
  return guard([&argc, &argv] { return runEval(argc, argv); });
}
