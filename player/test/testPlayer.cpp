#include <cassert>
#include <algorithm>
#include <iostream>
#include <vector>

#include "testPlayer.h"
#include "../Brain.h"
#include "../Player.h"
#include "../PlayerType.h"
#include "../Heuristic.h"
#include "../FeatureSet.h"
#include "../../duchess/EvaluationMode.h"

#include "../../duchess/Board.h"
#include "../../duchess/Position.h"
#include "../../duchess/Move.h"
#include "../../duchess/Piece.h"


void TestPlayer::runTests() {
    TestPlayer::test_playerGetterMethods();
    TestPlayer::test_AIReturnsAMove();
    TestPlayer::test_alphaBetaMakesSmartMoves();
}

void TestPlayer::test_playerGetterMethods() {
    Player p1 (PlayerType::AI, 3);
    assert(p1.getPlayerType() == PlayerType::AI);
    assert(p1.getPlayerNumber() == 3);
}

void TestPlayer::test_AIReturnsAMove() {
    Board b;
    Brain brain;
    brain.setHeuristic(Heuristic::RANDOM);
    b.initialiseAttDefVectors();

    Player p1 (PlayerType::AI, 1);
    p1.setBrain(&brain);

    std::vector<Move> validMoves = b.getLegalMoves(1);
    Move m = p1.chooseMove(&b, 1);
    assert(std::find(validMoves.begin(), validMoves.end(), m) != validMoves.end());
}

void TestPlayer::test_alphaBetaMakesSmartMoves() {
    Brain brain ("../weights/wcci/materialCheck.wts");
    brain.setHeuristic(Heuristic::KILLER_MOVE);
    brain.setFeatureSet(FeatureSet::MATERIAL);
    brain.setEvaluationMode(EvaluationMode::CUMULATIVE);

    Board board;
    // Move pawn 1c3 to 1c5
    // Move Queen 2c2 to 1b6, Knight 6b2 to 1d6
    // Make sure player 1 prefers to take the Queen using pawn1
    Position p1c3 ("1c3");
    Position p1c5 ("1c5");
    Position p1b6 ("1b6");
    Position p1d6 ("1d6");
    Position p2c2 ("2c2");
    Position p6b2 ("6b2");

    Piece* pawn1c3 = board.getPieceAtPosition(&p1c3);
    Piece* queen2c2 = board.getPieceAtPosition(&p2c2);
    Piece* knight6b2 = board.getPieceAtPosition(&p6b2);
    board.setPiecePosition(pawn1c3, &p1c5);
    board.setPiecePosition(queen2c2, &p1b6);
    board.setPiecePosition(knight6b2, &p1d6);
    board.initialiseAttDefVectors();

    Player p1 (PlayerType::AI, 1);
    p1.setBrain(&brain);

    Move m = p1.chooseMove(&board, 13);

    assert(m.getPieceTaken() == queen2c2);
    assert(*(m.getFromPosition()) == p1c5);
    assert(*(m.getToPosition()) == p1b6);

}