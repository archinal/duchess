#!/usr/bin/env bash

# Builds a visualisation script
# These scripts generate TiKz scripts
# Usage: ./build.sh [attack.cpp|board.cpp|position.cpp]

source ../buildScripts/sources.sh
source ../buildScripts/flags.sh

MAIN_FILE=$1
TARGET=${BIN_PATH}/`echo "${MAIN_FILE}" | sed -e "s/\.cpp$/\.o/"`


echo "Compiling to ${TARGET}"

g++ -std=${C_VERSION} ${PRODUCTION_FLAG_SET} -o ${TARGET} ${MAIN_FILE} ${DUCHESS_SOURCES} ${SERVER_SOURCES} ${PLAYER_SOURCES}