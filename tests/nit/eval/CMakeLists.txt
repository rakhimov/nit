include_directories("${PROJECT_SOURCE_DIR}/src/nit/eval/")

set(NIT_EVAL_TEST_SRC
  card_set_generators_test.cc
  card_set_test.cc
  holdem_hand_evaluator_test.cc
  omaha_high_hand_evaluator_test.cc
  poker_hand_evaluator_test.cc
  rank_test.cc
  suit_test.cc
  main.cc
  )
add_executable(eval_tests ${NIT_EVAL_TEST_SRC})
target_link_libraries(eval_tests nit)
add_test(TestEval ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/eval_tests)
