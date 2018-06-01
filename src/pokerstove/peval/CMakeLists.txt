# peval library
set(PEVAL_SRC
  card.cc
  card_set.cc
  card_set_generators.cc
  poker_evaluation.cc
  poker_hand.cc
  poker_hand_evaluator_alloc.cc
  poker_hand_evaluator.cc
  rank.cc
  suit.cc
  )
add_library(peval ${PEVAL_SRC})
