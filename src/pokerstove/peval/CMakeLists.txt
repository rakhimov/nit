find_package (Threads)

# build the test
set(PEVAL_TEST_SRC
  card_set_generators_test.cc
  card_set_test.cc
  holdem_hand_evaluator_test.cc
  omaha_high_hand_evaluator_test.cc
  poker_hand_evaluator_test.cc
  rank_test.cc
  suit_test.cc
  )
add_executable(peval_tests ${PEVAL_TEST_SRC})
target_link_libraries(peval_tests peval gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

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

add_test(TestPeval ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/peval_tests)
