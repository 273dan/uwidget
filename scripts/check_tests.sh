#!/bin/bash
if [[ $(basename $(pwd)) == "scripts" ]]; then
  echo "please run this with 'make check-tests' from the project root"
  exit
fi


RUNTIME_TESTS_DIR="./tests/runtime"
COMPILATION_TESTS_DIR="./tests/compilation"

RUNTIME_TESTS_CMAKELISTS="$RUNTIME_TESTS_DIR/CMakeLists.txt"
COMPILATION_TESTS_CMAKELISTS="$COMPILATION_TESTS_DIR/CMakeLists.txt"

RUNTIME_TEST_FN="uw_add_runtime_test("
COMPILATION_TEST_FN="uw_add_compilation_test("

has_missing_tests=0


for dir in RUNTIME COMPILATION; do
  declare -n directory="${dir}_TESTS_DIR"
  declare -n cmakelists="${dir}_TESTS_CMAKELISTS"
  declare -n fn="${dir}_TEST_FN"
  for test_src in $directory/*; do
    if [[ $test_src == $cmakelists ]]; then
      continue
    fi
    test_name=$(basename ${test_src%.*})
    grep_pattern="$fn$test_name"
    if [[ ! $(grep "$grep_pattern" $cmakelists) ]]; then
      echo "missing from $dir: $test_name"
      has_missing_tests=1
    fi
  done
done
if [[ $has_missing_tests == 0 ]]; then
  echo "no missing tests!"
fi
  
exit $has_missing_tests



