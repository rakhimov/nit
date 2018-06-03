include_directories("${PROJECT_SOURCE_DIR}/src/nit/enum/")

set(NIT_ENUM_TEST_SRC
  partition_enumerator_test.cc
  )
add_executable(enum_tests ${NIT_ENUM_TEST_SRC})
target_link_libraries(enum_tests nit)

add_test(TestEnum ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/enum_tests)
