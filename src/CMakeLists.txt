# check_function_exists does not work for builtins, so use try_compile.
try_compile(HAVE_BUILTIN_BITOPS
            "${PROJECT_BINARY_DIR}"
            "${CMAKE_CURRENT_SOURCE_DIR}/test_builtin_bitops.cc")

# Set up the configure file.
configure_file(config.h.in config.h)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
add_subdirectory(pokerstove/peval)
add_subdirectory(pokerstove/penum)
add_subdirectory(pokerstove/util)
add_subdirectory(ps-colex)
add_subdirectory(ps-eval)
add_subdirectory(ps-lut)
