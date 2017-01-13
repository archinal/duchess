#include "testHolisticTests.h"

#include "../Board.h"
#include "../Move.h"
#include "../Position.h"
#include "../Piece.h"
#include "../EvaluationMode.h"
#include "../MoveVector.h"

#include <vector>
#include <string>
#include <iostream>
#include <cassert>

void TestHolisticTests::runTests() {
    TestHolisticTests::test_linesThroughVortex();
    TestHolisticTests::test_kingOnVortex();
}

void TestHolisticTests::test_linesThroughVortex() {
    Board board;
    Position p1e1 ("1e1");
    Position p1e4 ("1e4");
    Position p6c6 ("6c6");
    Position p6d1 ("6d1");
    Position p2c2 ("2c2");
    Position p1c4 ("1c4");
    Position p2a2 ("2a2");
    Position p3c6 ("3c6");
    Position p4c1 ("4c1");
    Position p5b5 ("5b5");

    Piece* rook1e1 = board.getPieceAtPosition(&p1e1);
    Piece* fort6d1 = board.getPieceAtPosition(&p6d1);
    Piece* queen2c2 = board.getPieceAtPosition(&p2c2);
    Piece* bishop2a2 = board.getPieceAtPosition(&p2a2);
    Piece* king4 = board.getPieceAtPosition(&p4c1);

    board.setPiecePosition(fort6d1, &p6c6);
    board.setPiecePosition(rook1e1, &p1e4);
    board.setPiecePosition(queen2c2, &p1c4);
    board.setPiecePosition(bishop2a2, &p3c6);
    board.setPiecePosition(king4, &p5b5);

    board.initialiseAttDefVectors();

    Move move (&p1e4, &p6c6, fort6d1);

    TestHolisticTests::applyMoveAndUndoCheckingAllSame(board, move);

    assert(bishop2a2->getPassiveAttackingVectors().size() == 0);

    board.applyMoveCumulative(&move);

    assert(board.isPlayerInCheck(4));

    // Check pawn 3c3 is being defended by the rook
    assert(bishop2a2->getPassiveAttackingVectors().size() == 1);

}

void TestHolisticTests::applyMoveAndUndoCheckingAllSame(Board &board, const Move &move) {
    // Get move vectors before change
    std::vector <std::string> moveVectorStringsPre;
    // 2D array of player -> piece -> move vectors
    // For each piece we store its move vectors as a string
    short i, j;
    std::vector <std::vector<std::string>> pieceMoveVectorsPre;
    for (i = 1; i <= 6; ++i) {
        std::vector <std::string> vec;
        for (j = 0; j < 15; ++j) {
            vec.push_back(board.getPiecesForPlayer(i)[j]->getMoveVectorsAsString());
        }
        pieceMoveVectorsPre.push_back(vec);
    }

    // CHECK DENOVO
    // Make changes, undo
    board.applyMoveDeNovo(&move);
    board.undoMoveDeNovo(&move);

    // Get move vectors after change/undo
    std::vector <std::string> moveVectorStringsPost1;
    // 2D array of player -> piece -> move vectors
    // For each piece we store its move vectors as a string
    std::vector <std::vector<std::string>> pieceMoveVectorsPost1;
    for (i = 1; i <= 6; ++i) {
        std::vector <std::string> vec;
        for (j = 0; j < 15; ++j) {
            vec.push_back(board.getPiecesForPlayer(i)[j]->getMoveVectorsAsString());
        }
        pieceMoveVectorsPost1.push_back(vec);
    }

    // Check they're equal
    bool error = false;
    for (i = 0; i < 6; ++i) {
        for (j = 0; j < 15; ++j) {
            std::string expected = pieceMoveVectorsPre[i][j];
            std::string real = pieceMoveVectorsPost1[i][j];
            if (expected != real) {
                std::cout << "For piece: " << board.getPiecesForPlayer(i + 1)[j]->toString() << std::endl;
                std::cout << "Expected:" << std::endl;
                std::cout << expected << std::endl;
                std::cout << "Real:" << std::endl;
                std::cout << real << std::endl;
                error = true;
            }
        }
    }
    assert(!error);


    board.initialiseAttDefVectors();

    // CHECK CUMULATIVE
    // Make changes, undo
    board.applyMoveCumulative(&move);

    board.undoMoveCumulative(&move);

    // Get move vectors after change/undo
    std::vector <std::string> moveVectorStringsPost2;
    // 2D array of player -> piece -> move vectors
    // For each piece we store its move vectors as a string
    std::vector <std::vector<std::string>> pieceMoveVectorsPost2;
    for (i = 1; i <= 6; ++i) {
        std::vector <std::string> vec;
        for (j = 0; j < 15; ++j) {
            vec.push_back(board.getPiecesForPlayer(i)[j]->getMoveVectorsAsString());
        }
        pieceMoveVectorsPost2.push_back(vec);
    }

    // Check they're equal
    error = false;
    for (i = 0; i < 6; ++i) {
        for (j = 0; j < 15; ++j) {
            std::string expected = pieceMoveVectorsPre[i][j];
            std::string real = pieceMoveVectorsPost2[i][j];
            if (expected != real) {
                std::cout << "For piece: " << board.getPiecesForPlayer(i + 1)[j]->toString() << std::endl;
                std::cout << "Expected:" << std::endl;
                std::cout << expected << std::endl;
                std::cout << "Real:" << std::endl;
                std::cout << real << std::endl;
                error = true;
            }
        }
    }
    assert(!error);
}

void TestHolisticTests::test_kingOnVortex() {
    Board board;

    Position p1c1 ("1c1");
    Position p1e1 ("1e1");
    Position p1b6 ("1b6");
    Position p1a5 ("1a5");
    Position p2a2 ("2a2");
    Position vortex ("V");

    Piece* king1 = board.getPieceAtPosition(&p1c1);
    Piece* rook1 = board.getPieceAtPosition(&p1e1);
    Piece* bishop2 = board.getPieceAtPosition(&p2a2);

    board.setPiecePosition(king1, &vortex);
    board.setPiecePosition(rook1, &p1b6);
    board.setPiecePosition(bishop2, &p1a5);

    board.initialiseAttDefVectors();

    const std::vector<Move> moves = board.getLegalMoves(1);

    std::vector<Position*> line = {king1->getPosition(), rook1->getPosition(), bishop2->getPosition()};
    std::vector<Position*> attackingLineThroughPos = board.getAttackingLineOnKingThroughPosition(&p1b6, 1);
    assert(attackingLineThroughPos.size() == 3);
    assert(attackingLineThroughPos == line);

    // Make sure it's not legal to move the rook
    for (auto const& move : moves) {
        assert(move.getFromPosition()->toString() != "2d6");
    }

    board.setPiecePosition(bishop2, &p2a2);
    assert(board.getAttackingLineOnKingThroughPosition(&p1b6, 1).size() == 0);

    board.setPiecePosition(bishop2, &p1a5);
    board.setPiecePosition(rook1, &p1e1);

    assert(board.getAttackingLineOnKingThroughPosition(&p1e1, 1).size() == 0);

}