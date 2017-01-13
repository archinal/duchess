#!/usr/bin/env bash

DUCHESS_PATH="../duchess"
DUCHESS_TEST_PATH="${DUCHESS_PATH}/test"
GAME_PATH="../game"
GAME_TEST_PATH="${GAME_PATH}/test"
SERVER_PATH="../server"
SERVER_TEST_PATH="${SERVER_PATH}/test"
PLAYER_PATH="../player"
PLAYER_TEST_PATH="${PLAYER_PATH}/test"
BUILD_SRC_PATH="./"
BIN_PATH="../bin"
MAIN_PATH="../main"
SCRIPTS_PATH="../scripts"

# Test sources
DUCHESS_TEST_SOURCES="${DUCHESS_TEST_PATH}/testPosition.cpp
                ${DUCHESS_TEST_PATH}/testPositionUtils.cpp
                ${DUCHESS_TEST_PATH}/testPiece.cpp
                ${DUCHESS_TEST_PATH}/testBoard.cpp
                ${DUCHESS_TEST_PATH}/testHolisticTests.cpp
                ${DUCHESS_TEST_PATH}/testMove.cpp
                ${DUCHESS_TEST_PATH}/testTeamUtils.cpp
                ${DUCHESS_TEST_PATH}/testMoveVector.cpp
                ${DUCHESS_TEST_PATH}/testMoveVectorUtils.cpp"

PLAYER_TEST_SOURCES="${PLAYER_TEST_PATH}/testPlayer.cpp
                ${PLAYER_TEST_PATH}/testSearchAnalytics.cpp
                ${PLAYER_TEST_PATH}/testHeuristicCache.cpp
                ${PLAYER_TEST_PATH}/testFeatureUtils.cpp
                ${PLAYER_TEST_PATH}/testBrain.cpp"

# Duchess sources
DUCHESS_SOURCES="${DUCHESS_PATH}/Piece.cpp
                ${DUCHESS_PATH}/Position.cpp
                ${DUCHESS_PATH}/PositionUtils.cpp
                ${DUCHESS_PATH}/PositionUtilsLookupTables.cpp
                ${DUCHESS_PATH}/Board.cpp
                ${DUCHESS_PATH}/Move.cpp
                ${DUCHESS_PATH}/TeamUtils.cpp
                ${DUCHESS_PATH}/GeneralUtils.cpp
                ${DUCHESS_PATH}/MoveVector.cpp
                ${DUCHESS_PATH}/MoveVectorUtils.cpp"

# Game sources
GAME_SOURCES="${GAME_PATH}/Game.cpp
                ${GAME_PATH}/GameLog.cpp
                ${GAME_PATH}/GameSetLog.cpp"

GAME_TEST_SOURCES="${GAME_TEST_PATH}/testGameLog.cpp
                    ${GAME_TEST_PATH}/testGameSetLog.cpp"

# Player Sources
PLAYER_SOURCES="${PLAYER_PATH}/Player.cpp
                ${PLAYER_PATH}/FeatureUtils.cpp
                ${PLAYER_PATH}/SearchAnalytics.cpp
                ${PLAYER_PATH}/HeuristicCache.cpp
                ${PLAYER_PATH}/Brain.cpp"

SERVER_SOURCES="${SERVER_PATH}/Server.cpp
                ${SERVER_PATH}/LegacyUtils.cpp"

SERVER_TEST_SOURCES="${SERVER_TEST_PATH}/testLegacyUtils.cpp"