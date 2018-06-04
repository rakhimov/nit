cmake_minimum_required(VERSION 3.1)

# In-source build prevention.
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(NIT VERSION 0.0.0 LANGUAGES CXX)

option(NIT_WITH_COVERAGE "Instrument for coverage analysis" OFF)
option(NIT_BUILD_CLI "Build command-line tools" ON)
option(NIT_BUILD_TESTS "Build tests" ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # Needed for Clang Tooling.

# Require C++11 support.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror -Wnon-virtual-dtor -Wold-style-cast")

if(NIT_WITH_COVERAGE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wpedantic -Wredundant-decls -Wcast-align -Wlogical-op -Wvla -Wuseless-cast -Wunreachable-code -Wmissing-declarations")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "7.1")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wshadow -Wimplicit-fallthrough=0")
  endif()

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-missing-braces -Wshadow -Wunused-exception-parameter")
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

if(NIT_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

# Command-line tools
if(NIT_BUILD_CLI)
  add_subdirectory(cli)
endif()
