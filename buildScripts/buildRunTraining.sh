#!/bin/bash

set -e

source ./sources.sh
source ./flags.sh

EXECUTABLE_NAME="${BIN_PATH}/runTraining.o"
MAIN_FILE="${MAIN_PATH}/runTraining.cpp"

function compile_runTraining {
    echo "Compiling runTraining..."
    g++ ${PRODUCTION_FLAG_SET} -std="${C_VERSION}" -o "${EXECUTABLE_NAME}" "${MAIN_FILE}" ${DUCHESS_SOURCES} ${PLAYER_SOURCES} ${GAME_SOURCES}
}

compile_runTraining