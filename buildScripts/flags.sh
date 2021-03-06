#!/usr/bin/env bash

C_VERSION="c++11"
GCC_FLAGS="-Wall -Werror -lm"
DEBUGGING_FLAGS="-g -fno-inline"
OPTIMISATION_FLAGS="-O2"
PARALLEL_FLAGS=""

DEBUG_FLAG_SET="${GCC_FLAGS} ${DEBUGGING_FLAGS} ${PARALLEL_FLAGS}"
PRODUCTION_FLAG_SET="${GCC_FLAGS} ${OPTIMISATION_FLAGS} ${PARALLEL_FLAGS}"
