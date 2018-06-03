# Get scm detailed version.
execute_process(COMMAND git describe --tags --always
  OUTPUT_VARIABLE NIT_GIT_REVISION
  OUTPUT_STRIP_TRAILING_WHITESPACE)

# Set up the configure file.
configure_file(config.h.in nit/config.h)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
add_subdirectory(nit)
