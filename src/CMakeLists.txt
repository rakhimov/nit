# check_function_exists does not work for builtins, so use try_compile.
try_compile(HAVE_BUILTIN_BITOPS
            "${PROJECT_BINARY_DIR}"
            "${CMAKE_CURRENT_SOURCE_DIR}/test_builtin_bitops.cc")

# Get scm detailed version.
execute_process(COMMAND git describe --tags --always
  OUTPUT_VARIABLE NIT_GIT_REVISION
  OUTPUT_STRIP_TRAILING_WHITESPACE)

# Set up the configure file.
configure_file(config.h.in nit/config.h)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
add_subdirectory(nit)
