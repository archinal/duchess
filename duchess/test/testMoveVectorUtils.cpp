#include "testMoveVectorUtils.h"

#include "../Position.h"
#include "../Piece.h"
#include "../PieceType.h"
#include "../MoveVector.h"
#include "../Board.h"
#include "../MoveVectorUtils.h"

#include <cassert>
#include <vector>
#include <algorithm>

void TestMoveVectorUtils::runTests() {
    TestMoveVectorUtils::test_getCommonPositionsMoveVec();
    TestMoveVectorUtils::test_getCommonPositionsPos();
}

void TestMoveVectorUtils::test_getCommonPositionsMoveVec() {
    Board b;

    Position pos1 ("1a3");
    Position pos2 ("1e2");
    Piece* piece1 = b.getPieceAtPosition(&pos1);
    Piece* piece2 = b.getPieceAtPosition(&pos2);

    Position pos7 ("6d1"); // Player 6 fortress
    Piece* piece3 = b.getPieceAtPosition(&pos7);

    // Note this is not a true move vector in the sense that the positions listed do not really connect these pieces
    Position pos3 ("1c3");
    Position pos4 ("1d3");
    Position pos5 ("1e3");

    std::vector<Position*> squares1 = {&pos1, &pos2, &pos3};
    std::vector<Position*> squares2 = {&pos1, &pos2, &pos3, &pos4, &pos5};
    std::vector<Position*> squares3 = {&pos2, &pos3, &pos5};
    std::vector<Position*> squares4 = {&pos5};
    MoveVector* mv1 = new MoveVector (piece1, piece2, squares1);
    MoveVector* mv2 = new MoveVector (piece1, piece3, squares2);
    MoveVector* mv3 = new MoveVector (piece2, piece1, squares3);
    MoveVector* mv4 = new MoveVector (piece2, piece1, squares4);

    std::vector<MoveVector*> mvs1 = {mv1, mv2, mv3};
    std::vector<MoveVector*> mvs2 = {mv1, mv2};
    std::vector<MoveVector*> mvs3 = {mv1, mv4};
    std::vector<MoveVector*> mvs4 = {mv1};

    std::vector<Position*> positions1 = MoveVectorUtils::getCommonPositions(mvs1);
    std::vector<Position*> positions2 = MoveVectorUtils::getCommonPositions(mvs2);
    std::vector<Position*> positions3 = MoveVectorUtils::getCommonPositions(mvs3);
    std::vector<Position*> positions4 = MoveVectorUtils::getCommonPositions(mvs4);

    assert(std::find(positions1.begin(), positions1.end(), &pos2) != positions1.end());
    assert(std::find(positions1.begin(), positions1.end(), &pos3) != positions1.end());
    assert(positions1.size() == 2);

    assert(std::find(positions2.begin(), positions2.end(), &pos1) != positions2.end());
    assert(std::find(positions2.begin(), positions2.end(), &pos2) != positions2.end());
    assert(std::find(positions2.begin(), positions2.end(), &pos3) != positions2.end());
    assert(positions2.size() == 3);

    assert(positions3.size() == 0);

    assert(std::find(positions4.begin(), positions4.end(), &pos1) != positions4.end());
    assert(std::find(positions4.begin(), positions4.end(), &pos2) != positions4.end());
    assert(std::find(positions4.begin(), positions4.end(), &pos3) != positions4.end());
    assert(positions4.size() == 3);

    delete mv1;
    delete mv2;
    delete mv3;
    delete mv4;

}

void TestMoveVectorUtils::test_getCommonPositionsPos() {
    Position pos1 ("1a3");
    Position pos2 ("1e2");
    Position pos3 ("1c3");
    Position pos4 ("1d3");
    Position pos5 ("1e3");

    std::vector<Position*> squares1 = {&pos1, &pos2, &pos3};
    std::vector<Position*> squares2 = {&pos1, &pos2, &pos3, &pos4, &pos5};
    std::vector<Position*> squares3 = {&pos2, &pos3, &pos5};
    std::vector<Position*> squares4 = {&pos5};
    std::vector<Position*> squares5;

    std::vector<Position*> positions1 = MoveVectorUtils::getCommonPositions(squares1, squares3);
    std::vector<Position*> positions2 = MoveVectorUtils::getCommonPositions(squares1, squares2);
    std::vector<Position*> positions3 = MoveVectorUtils::getCommonPositions(squares1, squares4);
    std::vector<Position*> positions4 = MoveVectorUtils::getCommonPositions(squares1, squares5);

    assert(std::find(positions1.begin(), positions1.end(), &pos2) != positions1.end());
    assert(std::find(positions1.begin(), positions1.end(), &pos3) != positions1.end());
    assert(positions1.size() == 2);

    assert(std::find(positions2.begin(), positions2.end(), &pos1) != positions2.end());
    assert(std::find(positions2.begin(), positions2.end(), &pos2) != positions2.end());
    assert(std::find(positions2.begin(), positions2.end(), &pos3) != positions2.end());
    assert(positions2.size() == 3);

    assert(positions3.size() == 0);
    assert(positions4.size() == 0);
}