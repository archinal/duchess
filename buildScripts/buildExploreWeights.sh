#!/bin/bash

set -e

source ./sources.sh
source ./flags.sh

EXECUTABLE_NAME="${BIN_PATH}/exploreWeights.o"
MAIN_FILE="${SCRIPTS_PATH}/exploreWeights.cpp"

function compile_exploreWeights {
    echo "Compiling exploreWeights..."
    g++ ${GCC_FLAGS} -std="${C_VERSION}" -o "${EXECUTABLE_NAME}" "${MAIN_FILE}" ${DUCHESS_SOURCES} ${PLAYER_SOURCES}
}

compile_exploreWeights