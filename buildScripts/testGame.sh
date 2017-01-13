#!/usr/bin/env bash

source ./buildTestGame.sh

function run_tests {
    echo "Running tests"
    valgrind --leak-check=yes --quiet ./"${EXECUTABLE_NAME}"
}

run_tests