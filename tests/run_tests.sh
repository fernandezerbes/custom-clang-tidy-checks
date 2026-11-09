#!/usr/bin/env bash
# Usage: ./run_tests.sh <path-to-plugin-so>

PLUGIN="$1"
DIR="$(cd "$(dirname "$0")" && pwd)"
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

run_test() {
  local mode="$1" expected="$2"
  if clang-tidy -load="$PLUGIN" -checks=-*,bugprone-named-destructuring-check \
      -config="{CheckOptions: {bugprone-named-destructuring-check.ComparisonMode: $mode}}" \
      "$DIR/NamedDestructuringCheckExamples.cpp" 2>/dev/null | diff -q - "$DIR/$expected" >/dev/null; then
    echo -e "bugprone-named-destructuring-check $mode option ${GREEN}PASSED${NC}"
  else
    echo -e "bugprone-named-destructuring-check $mode option ${RED}FAILED${NC}"
  fi
}

run_test Exact test-data/NamedDestructuringCheckExactResult.out
run_test Fuzzy test-data/NamedDestructuringCheckFuzzyResult.out
