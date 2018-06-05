#!/usr/bin/env bash

set -ev
set -o pipefail

CLANG_FORMAT="./run-clang-format/run-clang-format.py --clang-format-executable clang-format-${CLANG_VERSION}"

${CLANG_FORMAT} -r src/
${CLANG_FORMAT} -r tests/
${CLANG_FORMAT} -r cli/
