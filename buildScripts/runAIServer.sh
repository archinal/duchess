#!/usr/bin/env bash

source ./buildAIServer.sh

PORT="9007"

function run_server {
    echo "Launching AI Server on port ${PORT}..."
    ./"${EXECUTABLE_NAME}" "${PORT}"
}

run_server