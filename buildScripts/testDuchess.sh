#!/usr/bin/env bash

source ./buildTestDuchess.sh

function run_tests {
    echo "Running tests"
    valgrind --leak-check=yes --quiet ./"${DUCHESS_EXECUTABLE_NAME}"
}

run_tests