cmake_minimum_required(VERSION 2.8)

project(nit CXX)

option(WITH_COVERAGE "Instrument for coverage analysis" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # Needed for Clang Tooling.

# TODO: Discover C++11 support.
# TODO: This flags only works for GCC compatible compilers (Clang, Intel).
#
# TODO: This, ideally, should be per library requirement,
#       but since all of them require C++11, the flag is set globally.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

if(WITH_COVERAGE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif()

#
# set up boost
#
set(BOOST_MIN_VERSION "1.46.0")
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED COMPONENTS program_options)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})
message(STATUS "Boost Include: ${Boost_INCLUDE_DIR}")
message(STATUS "Boost Libraries: ${Boost_LIBRARIES}")

# Also search for includes in PROJECT_BINARY_DIR to find config.h.
include_directories("${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
