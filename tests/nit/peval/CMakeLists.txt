include_directories("${PROJECT_SOURCE_DIR}/src/nit/peval/")

set(PEVAL_TEST_SRC
  card_set_generators_test.cc
  card_set_test.cc
  holdem_hand_evaluator_test.cc
  omaha_high_hand_evaluator_test.cc
  poker_hand_evaluator_test.cc
  rank_test.cc
  suit_test.cc
  main.cc
  )
add_executable(peval_tests ${PEVAL_TEST_SRC})
target_link_libraries(peval_tests peval)
add_test(TestPeval ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/peval_tests)
