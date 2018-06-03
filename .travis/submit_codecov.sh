#!/usr/bin/env bash

set -ev

# Submit coverage of C++
TRACE_FILE="coverage.info"
lcov --no-compat-libtool --directory build \
  -c --rc lcov_branch_coverage=1 -o $TRACE_FILE -q
lcov --extract $TRACE_FILE '*/nit/src/*' -o $TRACE_FILE
codecov > /dev/null
