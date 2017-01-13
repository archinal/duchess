#!/bin/bash

set -e

source ./sources.sh
source ./flags.sh

PLAYER_EXECUTABLE_NAME="${BIN_PATH}/testPlayer.o"
PLAYER_MAIN_FILE="${MAIN_PATH}/testPlayer.cpp"

function compile_for_test {
    echo "Compiling player..."
    g++ ${DEBUG_FLAG_SET} -std="${C_VERSION}" -o "${PLAYER_EXECUTABLE_NAME}" "${PLAYER_MAIN_FILE}" ${DUCHESS_SOURCES} ${PLAYER_SOURCES} ${PLAYER_TEST_SOURCES}
}

compile_for_test