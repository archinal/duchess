#!/bin/bash

set -e

source ./sources.sh
source ./flags.sh

EXECUTABLE_NAME="${BIN_PATH}/runExperiments.o"
MAIN_FILE="${MAIN_PATH}/runExperiments.cpp"

function compile_runExperiments {
    echo "Compiling runExperiments..."
    g++ ${PRODUCTION_FLAG_SET} -std="${C_VERSION}" -o "${EXECUTABLE_NAME}" "${MAIN_FILE}" ${DUCHESS_SOURCES} ${PLAYER_SOURCES} ${GAME_SOURCES}
}

compile_runExperiments