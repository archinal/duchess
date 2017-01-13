#include <assert.h>

#include <vector>
#include <algorithm>
#include <iostream>

#include "testPositionUtils.h"

#include "../Position.h"
#include "../PositionUtils.h"

void TestPositionUtils::runTests() {
    TestPositionUtils::test_boardIndexToPosition();
    TestPositionUtils::test_positionToBoardIndex();
    TestPositionUtils::test_getAdjacentSquareIndices();
    TestPositionUtils::test_getDiagonalSquareIndices();
    TestPositionUtils::test_getAdjacentAndDiagonalSquareIndices();

    TestPositionUtils::test_getIndexOfInnerClockwiseSquare();
    TestPositionUtils::test_getIndexOfOuterClockwiseSquare();
    TestPositionUtils::test_getIndexOfInnerAnticlockwiseSquare();
    TestPositionUtils::test_getIndexOfOuterAnticlockwiseSquare();
    TestPositionUtils::test_getIndexOfNextSquareInLine();
    TestPositionUtils::test_getKnightMovesFromPosition();
    TestPositionUtils::test_canPieceMoveInDirection();
    TestPositionUtils::test_distanceToVortex();
    TestPositionUtils::test_stepsToPosition();
}

void TestPositionUtils::test_boardIndexToPosition() {
    Position* p1 = PositionUtils::boardIndexToPositionSixPlayer(121); // 1c5
    Position* p2 = PositionUtils::boardIndexToPositionSixPlayer(47); // 3b2
    Position* p3 = PositionUtils::boardIndexToPositionSixPlayer(80); // 4e4
    Position* p4 = PositionUtils::boardIndexToPositionSixPlayer(1); // 1a1
    Position* p5 = PositionUtils::boardIndexToPositionSixPlayer(157); // V
    Position* p6 = PositionUtils::boardIndexToPositionSixPlayer(0); // OB
    Position* p7 = PositionUtils::boardIndexToPositionSixPlayer(142); // 4c6
    Position* p8 = PositionUtils::boardIndexToPositionSixPlayer(149); // 4b6
    Position* p9 = PositionUtils::boardIndexToPositionSixPlayer(147); // 5e5
    Position* p10 = PositionUtils::boardIndexToPositionSixPlayer(147); // 4a5
    Position* p11 = PositionUtils::boardIndexToPositionSixPlayer(120); // 6e4


    assert(*p1 == Position ("1c5"));
    assert(*p2 == Position ("3b2"));
    assert(*p3 == Position ("4e4"));
    assert(*p4 == Position ("1a1"));
    assert(*p5 == Position ("V"));
    assert(*p6 == Position ("OB"));
    assert(*p7 == Position ("4c6"));
    assert(*p8 == Position ("5d6"));
    assert(*p9 == Position ("5e5"));
    assert(*p9 == Position ("5e5"));
    assert(*p10 == Position ("4a5"));
    assert(*p11 == Position ("6e4"));

    delete p1;
    delete p2;
    delete p3;
    delete p4;
    delete p5;
    delete p6;
    delete p7;
    delete p8;
    delete p9;
    delete p10;
    delete p11;
};

void TestPositionUtils::test_positionToBoardIndex() {
    Position p1("1a1"); // 1
    Position p2("1a4"); // 16
    Position p3("6e4"); // 120
    Position p4("4e6"); // 144
    Position p5("3b5"); // 144
    Position p6("3a6"); // 140
    Position p7("4d5"); // 140
    Position p8("5d2"); // 89
    Position p9("OB"); // 0
    Position p10("V"); // 157
    Position p11("5c6"); // 148
    Position p12("1e6"); // 126

    assert(PositionUtils::positionToBoardIndexSixPlayer(&p1) == 1);
    assert(PositionUtils::positionToBoardIndexSixPlayer(&p2) == 16);
    assert(PositionUtils::positionToBoardIndexSixPlayer(&p3) == 120);
    assert(PositionUtils::positionToBoardIndexSixPlayer(&p4) == 144);
    assert(PositionUtils::positionToBoardIndexSixPlayer(&p5) == 144);
    assert(PositionUtils::positionToBoardIndexSixPlayer(&p6) == 140);
    assert(PositionUtils::positionToBoardIndexSixPlayer(&p7) == 140);
    assert(PositionUtils::positionToBoardIndexSixPlayer(&p8) == 89);
    assert(PositionUtils::positionToBoardIndexSixPlayer(&p9) == 0);
    assert(PositionUtils::positionToBoardIndexSixPlayer(&p10) == 157);
    assert(PositionUtils::positionToBoardIndexSixPlayer(&p11) == 148);
    assert(PositionUtils::positionToBoardIndexSixPlayer(&p12) == 126);
};

void TestPositionUtils::test_getAdjacentSquareIndices() {
    // Simple flap base case
    Position p1 ("1c2");
    std::vector<BoardIndex> adj1 = PositionUtils::getAdjacentSquareIndices(&p1);
    assert(std::find(adj1.begin(), adj1.end(), 3) != adj1.end());
    assert(std::find(adj1.begin(), adj1.end(), 7) != adj1.end());
    assert(std::find(adj1.begin(), adj1.end(), 9) != adj1.end());
    assert(std::find(adj1.begin(), adj1.end(), 13) != adj1.end());
    assert(adj1.size() == 4);

    // Bottom right corner
    Position p2 ("5e1");
    std::vector<BoardIndex> adj2 = PositionUtils::getAdjacentSquareIndices(&p2);
    assert(std::find(adj2.begin(), adj2.end(), 90) != adj2.end());
    assert(std::find(adj2.begin(), adj2.end(), 84) != adj2.end());
    assert(adj2.size() == 2);

    // Side of flap
    Position p3 ("3e3");
    std::vector<BoardIndex> adj3 = PositionUtils::getAdjacentSquareIndices(&p3);
    assert(std::find(adj3.begin(), adj3.end(), 60) != adj3.end());
    assert(std::find(adj3.begin(), adj3.end(), 54) != adj3.end());
    assert(std::find(adj3.begin(), adj3.end(), 50) != adj3.end());
    assert(adj3.size() == 3);

    // Top of flap in middle
    Position p4 ("4c5");
    std::vector<BoardIndex> adj4 = PositionUtils::getAdjacentSquareIndices(&p4);
    assert(std::find(adj4.begin(), adj4.end(), 142) != adj4.end());
    assert(std::find(adj4.begin(), adj4.end(), 140) != adj4.end());
    assert(std::find(adj4.begin(), adj4.end(), 150) != adj4.end());
    assert(std::find(adj4.begin(), adj4.end(), 78) != adj4.end());
    assert(adj4.size() == 4);

    Position p5 ("4c6");
    std::vector<BoardIndex> adj5 = PositionUtils::getAdjacentSquareIndices(&p5);
    assert(std::find(adj5.begin(), adj5.end(), 143) != adj5.end());
    assert(std::find(adj5.begin(), adj5.end(), 139) != adj5.end());
    assert(std::find(adj5.begin(), adj5.end(), 149) != adj5.end());
    assert(std::find(adj5.begin(), adj5.end(), 157) != adj5.end());
    assert(adj5.size() == 4);

    // Top of flap on side
    Position p6 ("6e6");
    std::vector<BoardIndex> adj6 = PositionUtils::getAdjacentSquareIndices(&p6);
    assert(std::find(adj6.begin(), adj6.end(), 145) != adj6.end());
    assert(std::find(adj6.begin(), adj6.end(), 155) != adj6.end());
    assert(std::find(adj6.begin(), adj6.end(), 153) != adj6.end());
    assert(std::find(adj6.begin(), adj6.end(), 97) != adj6.end());
    assert(adj6.size() == 4);

    // Vortex
    Position p7 ("V");
    std::vector<BoardIndex> adj7 = PositionUtils::getAdjacentSquareIndices(&p7);
    assert(std::find(adj7.begin(), adj7.end(), 124) != adj7.end());
    assert(std::find(adj7.begin(), adj7.end(), 130) != adj7.end());
    assert(std::find(adj7.begin(), adj7.end(), 136) != adj7.end());
    assert(std::find(adj7.begin(), adj7.end(), 142) != adj7.end());
    assert(std::find(adj7.begin(), adj7.end(), 148) != adj7.end());
    assert(std::find(adj7.begin(), adj7.end(), 154) != adj7.end());
    assert(adj7.size() == 6);

}

void TestPositionUtils::test_getDiagonalSquareIndices() {
    // Basic centre of flap
    Position p1 ("1c2");
    std::vector<BoardIndex> adj1 = PositionUtils::getDiagonalSquareIndices(&p1);
    assert(std::find(adj1.begin(), adj1.end(), 2) != adj1.end());
    assert(std::find(adj1.begin(), adj1.end(), 4) != adj1.end());
    assert(std::find(adj1.begin(), adj1.end(), 12) != adj1.end());
    assert(std::find(adj1.begin(), adj1.end(), 14) != adj1.end());
    assert(adj1.size() == 4);

    // Bottom corner of flap
    Position p2 ("6a1");
    std::vector<BoardIndex> adj2 = PositionUtils::getDiagonalSquareIndices(&p2);
    assert(std::find(adj2.begin(), adj2.end(), 107) != adj2.end());
    assert(adj2.size() == 1);

    // Middle side of flap
    Position p3 ("4e2");
    std::vector<BoardIndex> adj3 = PositionUtils::getDiagonalSquareIndices(&p3);
    assert(std::find(adj3.begin(), adj3.end(), 74) != adj3.end());
    assert(std::find(adj3.begin(), adj3.end(), 64) != adj3.end());
    assert(adj3.size() == 2);

    // Cross between flaps
    Position p4 ("3a4");
    std::vector<BoardIndex> adj4 = PositionUtils::getDiagonalSquareIndices(&p4);
    assert(std::find(adj4.begin(), adj4.end(), 80) != adj4.end());
    assert(std::find(adj4.begin(), adj4.end(), 144) != adj4.end());
    assert(std::find(adj4.begin(), adj4.end(), 52) != adj4.end());
    assert(adj4.size() == 3);

    Position p5 ("1e4");
    std::vector<BoardIndex> adj5 = PositionUtils::getDiagonalSquareIndices(&p5);
    assert(std::find(adj5.begin(), adj5.end(), 14) != adj5.end());
    assert(std::find(adj5.begin(), adj5.end(), 122) != adj5.end());
    assert(std::find(adj5.begin(), adj5.end(), 116) != adj5.end());
    assert(adj5.size() == 3);

    // Connecting vortex
    Position p6 ("1d6");
    std::vector<BoardIndex> adj6 = PositionUtils::getDiagonalSquareIndices(&p6);
    assert(std::find(adj6.begin(), adj6.end(), 157) != adj6.end());
    assert(std::find(adj6.begin(), adj6.end(), 121) != adj6.end());
    assert(std::find(adj6.begin(), adj6.end(), 123) != adj6.end());
    assert(std::find(adj6.begin(), adj6.end(), 151) != adj6.end());
    assert(adj6.size() == 4);

    // The vortex itself
    Position p7 ("V");
    std::vector<BoardIndex> adj7 = PositionUtils::getDiagonalSquareIndices(&p7);
    assert(std::find(adj7.begin(), adj7.end(), 125) != adj7.end());
    assert(std::find(adj7.begin(), adj7.end(), 131) != adj7.end());
    assert(std::find(adj7.begin(), adj7.end(), 137) != adj7.end());
    assert(std::find(adj7.begin(), adj7.end(), 143) != adj7.end());
    assert(std::find(adj7.begin(), adj7.end(), 149) != adj7.end());
    assert(std::find(adj7.begin(), adj7.end(), 155) != adj7.end());
    assert(adj7.size() == 6);

}

void TestPositionUtils::test_getAdjacentAndDiagonalSquareIndices() {
    Position p1("1c2");
    std::vector<BoardIndex> adj1 = PositionUtils::getAdjacentAndDiagonalSquareIndices(&p1);
    assert(std::find(adj1.begin(), adj1.end(), 2) != adj1.end());
    assert(std::find(adj1.begin(), adj1.end(), 3) != adj1.end());
    assert(std::find(adj1.begin(), adj1.end(), 4) != adj1.end());
    assert(std::find(adj1.begin(), adj1.end(), 7) != adj1.end());
    assert(std::find(adj1.begin(), adj1.end(), 9) != adj1.end());
    assert(std::find(adj1.begin(), adj1.end(), 12) != adj1.end());
    assert(std::find(adj1.begin(), adj1.end(), 13) != adj1.end());
    assert(std::find(adj1.begin(), adj1.end(), 14) != adj1.end());
    assert(adj1.size() == 8);

    Position p2("1a1");
    std::vector<BoardIndex> adj2 = PositionUtils::getAdjacentAndDiagonalSquareIndices(&p2);
    assert(std::find(adj2.begin(), adj2.end(), 2) != adj2.end());
    assert(std::find(adj2.begin(), adj2.end(), 6) != adj2.end());
    assert(std::find(adj2.begin(), adj2.end(), 7) != adj2.end());
    assert(adj2.size() == 3);

    Position p3("V");
    std::vector<BoardIndex> adj3 = PositionUtils::getAdjacentAndDiagonalSquareIndices(&p3);
    assert(std::find(adj3.begin(), adj3.end(), 125) != adj3.end());
    assert(std::find(adj3.begin(), adj3.end(), 124) != adj3.end());
    assert(std::find(adj3.begin(), adj3.end(), 131) != adj3.end());
    assert(std::find(adj3.begin(), adj3.end(), 130) != adj3.end());
    assert(std::find(adj3.begin(), adj3.end(), 137) != adj3.end());
    assert(std::find(adj3.begin(), adj3.end(), 136) != adj3.end());
    assert(std::find(adj3.begin(), adj3.end(), 143) != adj3.end());
    assert(std::find(adj3.begin(), adj3.end(), 142) != adj3.end());
    assert(std::find(adj3.begin(), adj3.end(), 149) != adj3.end());
    assert(std::find(adj3.begin(), adj3.end(), 148) != adj3.end());
    assert(std::find(adj3.begin(), adj3.end(), 155) != adj3.end());
    assert(std::find(adj3.begin(), adj3.end(), 154) != adj3.end());
    assert(adj3.size() == 12);
}

void TestPositionUtils::test_getIndexOfInnerClockwiseSquare() {
    Position p1("1c2");
    assert(PositionUtils::getIndexOfInnerClockwiseSquare(&p1) == 12);

    Position p2("1c4");
    assert(PositionUtils::getIndexOfInnerClockwiseSquare(&p2) == 132);

    Position p3("1a4");
    assert(PositionUtils::getIndexOfInnerClockwiseSquare(&p3) == 40);

    Position p4("1c6");
    assert(PositionUtils::getIndexOfInnerClockwiseSquare(&p4) == 130);
}

void TestPositionUtils::test_getIndexOfOuterClockwiseSquare() {
    Position p1("1c2");
    assert(PositionUtils::getIndexOfOuterClockwiseSquare(&p1) == 2);

    Position p2("1c6");
    assert(PositionUtils::getIndexOfOuterClockwiseSquare(&p2) == 132);

}

void TestPositionUtils::test_getIndexOfInnerAnticlockwiseSquare() {
    Position p1("1c2");
    assert(PositionUtils::getIndexOfInnerAnticlockwiseSquare(&p1) == 14);

    Position p2("1e4");
    assert(PositionUtils::getIndexOfInnerAnticlockwiseSquare(&p2) == 116);

    Position p3("1a4");
    assert(PositionUtils::getIndexOfInnerAnticlockwiseSquare(&p3) == 132);

    Position p4("1c6");
    assert(PositionUtils::getIndexOfInnerAnticlockwiseSquare(&p4) == 154);
}

void TestPositionUtils::test_getIndexOfOuterAnticlockwiseSquare() {
    Position p1("1c2");
    assert(PositionUtils::getIndexOfOuterAnticlockwiseSquare(&p1) == 4);

    Position p2("1c6");
    assert(PositionUtils::getIndexOfOuterAnticlockwiseSquare(&p2) == 122);

    Position p3("1e6");
    assert(PositionUtils::getIndexOfOuterAnticlockwiseSquare(&p3) == 116);
}

void TestPositionUtils::test_getIndexOfNextSquareInLine() {
    // Simple cases on a flap area
    Position p1 ("1c3");

    Position p2 ("1c2");
    Position p3 ("1b3");
    Position p4 ("1d3");
    Position p5 ("1c4");

    Position p6 ("1b4");
    Position p7 ("1d4");
    Position p8 ("1b2");
    Position p9 ("1d2");

    assert(PositionUtils::getIndexOfNextSquareInLine(&p1, &p2, PositionUtils::DIRECTION_OUT) == 3);
    assert(PositionUtils::getIndexOfNextSquareInLine(&p1, &p3, PositionUtils::DIRECTION_CLOCKWISE) == 11);
    assert(PositionUtils::getIndexOfNextSquareInLine(&p1, &p4, PositionUtils::DIRECTION_ANTICLOCKWISE) == 15);
    assert(PositionUtils::getIndexOfNextSquareInLine(&p1, &p5, PositionUtils::DIRECTION_IN) == 121);

    assert(PositionUtils::getIndexOfNextSquareInLine(&p1, &p6, PositionUtils::DIRECTION_IN_CLOCKWISE) == 129);
    assert(PositionUtils::getIndexOfNextSquareInLine(&p1, &p7, PositionUtils::DIRECTION_IN_ANTICLOCKWISE) == 123);
    assert(PositionUtils::getIndexOfNextSquareInLine(&p1, &p8, PositionUtils::DIRECTION_OUT_CLOCKWISE) == 1);
    assert(PositionUtils::getIndexOfNextSquareInLine(&p1, &p9, PositionUtils::DIRECTION_OUT_ANTICLOCKWISE) == 5);

    // Test off board
    Position p10 ("1a3");
    assert(PositionUtils::getIndexOfNextSquareInLine(&p3, &p10, PositionUtils::DIRECTION_CLOCKWISE) == 0);


    // Test where vortex is first pos
    Position vortex ("V");
    Position p11 ("4c6");
    Position p12 ("4d6");
    assert(PositionUtils::getIndexOfNextSquareInLine(&vortex, &p11, PositionUtils::DIRECTION_OUT) == 139);
    assert(PositionUtils::getIndexOfNextSquareInLine(&vortex, &p12, PositionUtils::DIRECTION_OUT_ANTICLOCKWISE) == 141);


    // Test where vortex is second pos
    assert(PositionUtils::getIndexOfNextSquareInLine(&p11, &vortex, PositionUtils::DIRECTION_IN) == 124);
    assert(PositionUtils::getIndexOfNextSquareInLine(&p12, &vortex, PositionUtils::DIRECTION_IN_CLOCKWISE) == 125);

    // Test where crossing over flap boundary
    Position p13("1d6");
    Position p14 ("6c6"); // IN
    Position p15 ("6c5"); // IN_ANTICLOCKWISE
    assert(PositionUtils::getIndexOfNextSquareInLine(&p13, &p14, PositionUtils::DIRECTION_IN) == 155);
    assert(PositionUtils::getIndexOfNextSquareInLine(&p13, &p15, PositionUtils::DIRECTION_IN_ANTICLOCKWISE) == 119);

    Position p16("1b4");
    Position p17("1b5"); // IN
    Position p18("1a5"); // IN_CLOCKWISE
    assert(PositionUtils::getIndexOfNextSquareInLine(&p16, &p17, PositionUtils::DIRECTION_IN) == 131);
    assert(PositionUtils::getIndexOfNextSquareInLine(&p16, &p18, PositionUtils::DIRECTION_IN_CLOCKWISE) == 39);

    Position p19("1c6");
    Position p20("1b6"); // CLOCKWISE
    Position p21("1b5"); // OUT_CLOCKWISE
    assert(PositionUtils::getIndexOfNextSquareInLine(&p19, &p20, PositionUtils::DIRECTION_CLOCKWISE) == 128);
    assert(PositionUtils::getIndexOfNextSquareInLine(&p19, &p21, PositionUtils::DIRECTION_OUT_CLOCKWISE) == 16);

    Position p22("1a4");
    Position p23("1b5"); // IN_ANTI_CLOCKWISE
    assert(PositionUtils::getIndexOfNextSquareInLine(&p22, &p23, PositionUtils::DIRECTION_IN_ANTICLOCKWISE) == 124);

    Position p24("1e6");
    Position p25("6a5"); // OUT_ANTICLOCKWISE
    Position p26("6c6"); // IN_CLOCK
    assert(PositionUtils::getIndexOfNextSquareInLine(&p24, &p25, PositionUtils::DIRECTION_OUT_ANTICLOCKWISE) == 0);
    assert(PositionUtils::getIndexOfNextSquareInLine(&p24, &p26, PositionUtils::DIRECTION_IN_CLOCKWISE) == 148);

}

void TestPositionUtils::test_getKnightMovesFromPosition() {

    // Basic case in flap
    Position p1("1c2");
    std::vector<BoardIndex> p1Moves = PositionUtils::getKnightMovesFromPosition(&p1);
    assert(std::find(p1Moves.begin(), p1Moves.end(), 1) != p1Moves.end());
    assert(std::find(p1Moves.begin(), p1Moves.end(), 5) != p1Moves.end());
    assert(std::find(p1Moves.begin(), p1Moves.end(), 11) != p1Moves.end());
    assert(std::find(p1Moves.begin(), p1Moves.end(), 15) != p1Moves.end());
    assert(std::find(p1Moves.begin(), p1Moves.end(), 17) != p1Moves.end());
    assert(std::find(p1Moves.begin(), p1Moves.end(), 19) != p1Moves.end());
    assert(p1Moves.size() == 6);

    // Bottom left corner
    Position p2("1b1");
    std::vector<BoardIndex> p2Moves = PositionUtils::getKnightMovesFromPosition(&p2);
    assert(std::find(p2Moves.begin(), p2Moves.end(), 11) != p2Moves.end());
    assert(std::find(p2Moves.begin(), p2Moves.end(), 13) != p2Moves.end());
    assert(std::find(p2Moves.begin(), p2Moves.end(), 9) != p2Moves.end());
    assert(p2Moves.size() == 3);

    // Case on end of flap
    Position p3("1c5");
    std::vector<BoardIndex> p3Moves = PositionUtils::getKnightMovesFromPosition(&p3);
    assert(std::find(p3Moves.begin(), p3Moves.end(), 16) != p3Moves.end());
    assert(std::find(p3Moves.begin(), p3Moves.end(), 20) != p3Moves.end());
    assert(std::find(p3Moves.begin(), p3Moves.end(), 12) != p3Moves.end());
    assert(std::find(p3Moves.begin(), p3Moves.end(), 14) != p3Moves.end());
    assert(std::find(p3Moves.begin(), p3Moves.end(), 128) != p3Moves.end());
    assert(std::find(p3Moves.begin(), p3Moves.end(), 126) != p3Moves.end());
    assert(std::find(p3Moves.begin(), p3Moves.end(), 130) != p3Moves.end());
    assert(std::find(p3Moves.begin(), p3Moves.end(), 154) != p3Moves.end());
    assert(std::find(p3Moves.begin(), p3Moves.end(), 136) != p3Moves.end());
    assert(std::find(p3Moves.begin(), p3Moves.end(), 148) != p3Moves.end());
    assert(p3Moves.size() == 10);

    Position p4("2c6");
    std::vector<BoardIndex> p4Moves = PositionUtils::getKnightMovesFromPosition(&p4);
    assert(std::find(p4Moves.begin(), p4Moves.end(), 37) != p4Moves.end());
    assert(std::find(p4Moves.begin(), p4Moves.end(), 39) != p4Moves.end());
    assert(std::find(p4Moves.begin(), p4Moves.end(), 135) != p4Moves.end());
    assert(std::find(p4Moves.begin(), p4Moves.end(), 129) != p4Moves.end());
    assert(std::find(p4Moves.begin(), p4Moves.end(), 133) != p4Moves.end());
    assert(std::find(p4Moves.begin(), p4Moves.end(), 121) != p4Moves.end());
    assert(std::find(p4Moves.begin(), p4Moves.end(), 125) != p4Moves.end());
    assert(std::find(p4Moves.begin(), p4Moves.end(), 143) != p4Moves.end());
    assert(std::find(p4Moves.begin(), p4Moves.end(), 149) != p4Moves.end());
    assert(std::find(p4Moves.begin(), p4Moves.end(), 155) != p4Moves.end());
    assert(std::find(p4Moves.begin(), p4Moves.end(), 139) != p4Moves.end());
    assert(std::find(p4Moves.begin(), p4Moves.end(), 151) != p4Moves.end());
    assert(p4Moves.size() == 12);

    // Starting on vortex
    Position vortex("V");
    std::vector<BoardIndex> vortexMoves = PositionUtils::getKnightMovesFromPosition(&vortex);
    assert(std::find(vortexMoves.begin(), vortexMoves.end(), 126) != vortexMoves.end());
    assert(std::find(vortexMoves.begin(), vortexMoves.end(), 122) != vortexMoves.end());
    assert(std::find(vortexMoves.begin(), vortexMoves.end(), 132) != vortexMoves.end());
    assert(std::find(vortexMoves.begin(), vortexMoves.end(), 128) != vortexMoves.end());
    assert(std::find(vortexMoves.begin(), vortexMoves.end(), 138) != vortexMoves.end());
    assert(std::find(vortexMoves.begin(), vortexMoves.end(), 134) != vortexMoves.end());
    assert(std::find(vortexMoves.begin(), vortexMoves.end(), 144) != vortexMoves.end());
    assert(std::find(vortexMoves.begin(), vortexMoves.end(), 140) != vortexMoves.end());
    assert(std::find(vortexMoves.begin(), vortexMoves.end(), 150) != vortexMoves.end());
    assert(std::find(vortexMoves.begin(), vortexMoves.end(), 146) != vortexMoves.end());
    assert(std::find(vortexMoves.begin(), vortexMoves.end(), 156) != vortexMoves.end());
    assert(std::find(vortexMoves.begin(), vortexMoves.end(), 152) != vortexMoves.end());
    assert(vortexMoves.size() == 12);

}

void TestPositionUtils::test_canPieceMoveInDirection() {
    assert(!PositionUtils::canPieceMoveInDirection(PieceType::BISHOP, PositionUtils::DIRECTION_ANTICLOCKWISE));
    assert(PositionUtils::canPieceMoveInDirection(PieceType::BISHOP, PositionUtils::DIRECTION_OUT_ANTICLOCKWISE));
    assert(PositionUtils::canPieceMoveInDirection(PieceType::QUEEN, PositionUtils::DIRECTION_IN_ANTICLOCKWISE));
    assert(PositionUtils::canPieceMoveInDirection(PieceType::FORTRESS, PositionUtils::DIRECTION_ANTICLOCKWISE));
    assert(PositionUtils::canPieceMoveInDirection(PieceType::ROOK, PositionUtils::DIRECTION_IN));
    assert(!PositionUtils::canPieceMoveInDirection(PieceType::FORTRESS, PositionUtils::DIRECTION_IN_ANTICLOCKWISE));
}

void TestPositionUtils::test_distanceToVortex() {
    Position p1 ("1a1"); // 8
    Position p2 ("1e1"); // 8
    Position p3 ("2c6"); // 1
    Position p4 ("2e6"); // 3
    Position p5 ("2b6"); // 2
    Position p6 ("4d3"); // 5
    Position vortex ("V"); // 0

    assert(PositionUtils::distanceToVortex(&p1) == 8);
    assert(PositionUtils::distanceToVortex(&p2) == 8);
    assert(PositionUtils::distanceToVortex(&p3) == 1);
    assert(PositionUtils::distanceToVortex(&p4) == 3);
    assert(PositionUtils::distanceToVortex(&p5) == 2);
    assert(PositionUtils::distanceToVortex(&p6) == 5);
    assert(PositionUtils::distanceToVortex(&vortex) == 0);

}

void TestPositionUtils::test_stepsToPosition() {
    Position p1a1 ("1a1");
    Position p1a2 ("1a2");
    Position p1b3 ("1b3");
    Position p2e1 ("2e1"); // 4 knight moves from 1a1 but only 2 rook moves
    assert(PositionUtils::stepsToPosition(0, &p1a1, &p1a2) == 1);
    assert(PositionUtils::stepsToPosition(0, &p1a1, &p1a1) == 0);
    assert(PositionUtils::stepsToPosition(0, &p1a1, &p1b3) == 3);
    assert(PositionUtils::stepsToPosition(2, &p1a1, &p2e1) == 4);
    assert(PositionUtils::stepsToPosition(4, &p1a1, &p2e1) == 2);
    assert(PositionUtils::stepsToPosition(7, &p1a1, &p2e1) == 2);
}
