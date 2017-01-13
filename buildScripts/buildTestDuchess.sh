#!/bin/bash

set -e

source ./sources.sh
source ./flags.sh

DUCHESS_EXECUTABLE_NAME="${BIN_PATH}/testDuchess.o"
DUCHESS_MAIN_FILE="${MAIN_PATH}/testDuchess.cpp"

function compile_for_test {
    echo "Compiling duchess..."
    g++ ${DEBUG_FLAG_SET} -std="${C_VERSION}" -o "${DUCHESS_EXECUTABLE_NAME}" "${DUCHESS_MAIN_FILE}" ${DUCHESS_SOURCES} ${DUCHESS_TEST_SOURCES}
}

compile_for_test