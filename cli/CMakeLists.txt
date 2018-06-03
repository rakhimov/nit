include_directories("${PROJECT_SOURCE_DIR}/src")

add_executable(nit-colex nit-colex.cc)
target_link_libraries(nit-colex nit ${Boost_LIBRARIES})

add_executable(nit-eval nit-eval.cc)
target_link_libraries(nit-eval nit ${Boost_LIBRARIES})

add_executable(nit-lut nit-lut.cc)
target_link_libraries(nit-lut nit ${Boost_LIBRARIES})
