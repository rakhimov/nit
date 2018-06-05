set(NIT_SRC
  # enumeration
  enum/card_distribution.cc
  enum/showdown_enumerator.cc
  # evaluation
  eval/card.cc
  eval/card_set.cc
  eval/card_set_generators.cc
  eval/poker_evaluation.cc
  eval/poker_hand.cc
  eval/make_evaluator.cc
  eval/poker_hand_evaluator.cc
  eval/rank.cc
  eval/suit.cc
  )
add_library(nit ${NIT_SRC})
