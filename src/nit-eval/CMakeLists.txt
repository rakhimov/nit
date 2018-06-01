add_executable(nit-eval main.cc)
target_link_libraries(nit-eval peval penum ${Boost_LIBRARIES})
