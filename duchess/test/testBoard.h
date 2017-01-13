#ifndef DUCHESS_TESTBOARD_H
#define DUCHESS_TESTBOARD_H

class TestBoard {
public:
    static void runTests();

private:
    static void test_constructor();
    static void test_getPieceAtPosition();
    static void test_getPosition();
    static void test_getPiecesForPlayer();
    static void test_isPieceWizardAdjacent();
    static void test_doesSquareContain();
    static void test_getAccessibleSquares();
    static void test_getAccessibleSquaresOnVortex();
    static void test_setPiecePosition();
    static void test_initialiseAttDefVectors();
    static void test_getAttackingLineOnKingThroughPosition();
    static void test_getLegalMoves();
    static void test_applyMoveCumulative();
    static void test_applyMoveWithCheckCumulative();
    static void test_applyMoveDeNovo();
    static void test_applyMoveWithCheckDeNovo();
    static void test_getLinesAttackingSquare();
    static void test_getActiveLinesFromVortex();
    static void test_undoMove();
    static void test_moveVectorsThroughVortex();

};

#endif //DUCHESS_TESTBOARD_H
