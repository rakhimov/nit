include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/deps/catch2/single_include")

include_directories("${PROJECT_SOURCE_DIR}/src/")

add_subdirectory(nit/eval)
add_subdirectory(nit/enum)
add_subdirectory(nit/util)
