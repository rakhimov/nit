include_directories("${PROJECT_SOURCE_DIR}/src/nit/penum/")

set(PENUM_TEST_SRC
  partition_enumerator_test.cc
  )
add_executable(penum_tests ${PENUM_TEST_SRC})
target_link_libraries(penum_tests penum peval)

add_test(TestPenum ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/penum_tests)
