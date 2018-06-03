set(NIT_SRC
  # enumeration
  penum/card_distribution.cc
  penum/showdown_enumerator.cc
  # evaluation
  peval/card.cc
  peval/card_set.cc
  peval/card_set_generators.cc
  peval/poker_evaluation.cc
  peval/poker_hand.cc
  peval/poker_hand_evaluator_alloc.cc
  peval/poker_hand_evaluator.cc
  peval/rank.cc
  peval/suit.cc
  )
add_library(nit ${NIT_SRC})
