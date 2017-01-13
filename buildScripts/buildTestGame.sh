#!/bin/bash

set -e

source ./sources.sh
source ./flags.sh

EXECUTABLE_NAME="${BIN_PATH}/testGame.o"
MAIN_FILE="${MAIN_PATH}/testGame.cpp"

function compile_for_test {
    echo "Compiling game..."
    g++ ${DEBUG_FLAG_SET} -std="${C_VERSION}" -o "${EXECUTABLE_NAME}" "${MAIN_FILE}" ${DUCHESS_SOURCES} ${GAME_SOURCES} ${GAME_TEST_SOURCES} ${PLAYER_SOURCES}
}

compile_for_test