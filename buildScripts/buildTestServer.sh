#!/usr/bin/env bash

set -e

source ./sources.sh
source ./flags.sh

EXECUTABLE_NAME="${BIN_PATH}/testServer.o"
MAIN_FILE="${MAIN_PATH}/testServer.cpp"

function compile_rungame {
    echo "Compiling testAIServer..."
    g++ ${DEBUG_FLAG_SET} -std="${C_VERSION}" -o "${EXECUTABLE_NAME}" "${MAIN_FILE}" ${DUCHESS_SOURCES} ${PLAYER_SOURCES} ${GAME_SOURCES} ${SERVER_SOURCES} ${SERVER_TEST_SOURCES}
}

compile_rungame