#ifndef DUCHESS_TESTPOSITIONUTILS_H
#define DUCHESS_TESTPOSITIONUTILS_H

class TestPositionUtils {
public:
    static void runTests();

private:
    static void test_boardIndexToPosition();
    static void test_positionToBoardIndex();
    static void test_getAdjacentSquareIndices();
    static void test_getDiagonalSquareIndices();
    static void test_getAdjacentAndDiagonalSquareIndices();
    static void test_getIndexOfInnerClockwiseSquare();
    static void test_getIndexOfOuterClockwiseSquare();
    static void test_getIndexOfInnerAnticlockwiseSquare();
    static void test_getIndexOfOuterAnticlockwiseSquare();
    static void test_getIndexOfNextSquareInLine();
    static void test_getKnightMovesFromPosition();
    static void test_canPieceMoveInDirection();
    static void test_distanceToVortex();
    static void test_stepsToPosition();
};

#endif //DUCHESS_TESTPOSITIONUTILS_H
