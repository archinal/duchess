#!/bin/bash

set -e

source ./sources.sh
source ./flags.sh

EXECUTABLE_NAME="${BIN_PATH}/generateRelativePositionLookupTables.o"
MAIN_FILE="${SCRIPTS_PATH}/generateRelativePositionLookupTables.cpp"

function compile_genlookup {
    echo "Compiling genLookups..."
    g++ ${GCC_FLAGS} -std="${C_VERSION}" -o "${EXECUTABLE_NAME}" "${MAIN_FILE}" ${DUCHESS_SOURCES} ${PLAYER_SOURCES}
}

compile_genlookup