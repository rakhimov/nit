cmake_minimum_required(VERSION 2.8)
find_package (Threads)

# build the test
set(PENUM_TEST_SRC
  partition_enumerator_test.cc
  simple_deck_test.cc
  )
add_executable(penum_tests ${PENUM_TEST_SRC})
target_link_libraries(penum_tests penum peval gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

# penum library
set(PENUM_SRC
  card_distribution.cc
  showdown_enumerator.cc
  )

add_library(penum ${PENUM_SRC})

add_test(TestPenum ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/penum_tests)
