#!/usr/bin/env bash

set -ev

# Submit coverage of C++
PENUM_COV_DIR="build/src/pokerstove/penum/CMakeFiles/penum.dir/"  # TODO: Enable
PENUM_TESTS_COV_DIR="build/tests/pokerstove/penum/CMakeFiles/penum_tests.dir/"
PEVAL_COV_DIR="build/src/pokerstove/peval/CMakeFiles/peval.dir/"
PEVAL_TESTS_COV_DIR="build/tests/pokerstove/peval/CMakeFiles/peval_tests.dir/"
UTIL_TESTS_COV_DIR="build/tests/pokerstove/util/CMakeFiles/util_tests.dir/"
TRACE_FILE="coverage.info"
lcov --no-compat-libtool \
  --directory $PENUM_TESTS_COV_DIR \
  --directory $PEVAL_COV_DIR \
  --directory $PEVAL_TESTS_COV_DIR \
  --directory $UTIL_TESTS_COV_DIR \
  -c --rc lcov_branch_coverage=1 -o $TRACE_FILE -q
lcov --extract $TRACE_FILE '*/nit/src/*' -o $TRACE_FILE
codecov > /dev/null
