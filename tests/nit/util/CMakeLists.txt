include_directories("${PROJECT_SOURCE_DIR}/src/nit/util/")

set(UTIL_TEST_SRC
  lastbit_test.cc
  )
add_executable(util_tests ${UTIL_TEST_SRC})
add_test(TestUtil ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/util_tests)
