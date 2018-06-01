include_directories("${PROJECT_SOURCE_DIR}/src/pokerstove/util/")

set(UTIL_TEST_SRC
  lastbit_test.cc
  )
add_executable(util_tests ${UTIL_TEST_SRC})
target_link_libraries(util_tests peval gtest gtest_main)

add_test(TestUtil ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/util_tests)
