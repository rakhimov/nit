include_directories("${PROJECT_SOURCE_DIR}/src/pokerstove/penum/")

find_package (Threads)

set(PENUM_TEST_SRC
  partition_enumerator_test.cc
  simple_deck_test.cc
  )
add_executable(penum_tests ${PENUM_TEST_SRC})
target_link_libraries(penum_tests penum peval gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

add_test(TestPenum ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/penum_tests)
