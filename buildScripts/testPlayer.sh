#!/usr/bin/env bash

source ./buildTestPlayer.sh

function run_tests {
    echo "Running tests"
    valgrind --leak-check=yes --quiet ./"${PLAYER_EXECUTABLE_NAME}"
}

run_tests