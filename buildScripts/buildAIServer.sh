#!/usr/bin/env bash

set -e

source ./sources.sh
source ./flags.sh

EXECUTABLE_NAME="${BIN_PATH}/launchAISever.o"
MAIN_FILE="${MAIN_PATH}/launchAIServer.cpp"

function compile_rungame {
    echo "Compiling launchAIServer..."
    g++ ${PRODUCTION_FLAG_SET} -std="${C_VERSION}" -o "${EXECUTABLE_NAME}" "${MAIN_FILE}" ${DUCHESS_SOURCES} ${PLAYER_SOURCES} ${GAME_SOURCES} ${SERVER_SOURCES}
}

compile_rungame