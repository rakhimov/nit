include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/deps/catch2/single_include")

include_directories("${PROJECT_SOURCE_DIR}/src/")

add_subdirectory(pokerstove/peval)
add_subdirectory(pokerstove/penum)
add_subdirectory(pokerstove/util)
