include_directories("${PROJECT_SOURCE_DIR}/src")

add_executable(nit-colex nit-colex.cc)
target_link_libraries(nit-colex peval ${Boost_LIBRARIES})

add_executable(nit-eval nit-eval.cc)
target_link_libraries(nit-eval peval penum ${Boost_LIBRARIES})

add_executable(nit-lut nit-lut.cc)
target_link_libraries(nit-lut peval ${Boost_LIBRARIES})
