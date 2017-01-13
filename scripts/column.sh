#!/usr/bin/env bash

# Usage:
# $1: File to search
# $2: Col to search
# $3: Start row
# $4: End row

set -e

FILE_NAME=$1
COLUMN=$2
START_ROW=$3
NUM_ROWS=$4


FILE_LENGTH="$(wc -l ${FILE_NAME} | cut -d " " -f 1)"
LOG_LINES="$((FILE_LENGTH-(1+START_ROW)))"


tail -n "${LOG_LINES}" "${FILE_NAME}" | head -n "${NUM_ROWS}" | cut -d',' -f ${COLUMN} | sed 's/ //g'

