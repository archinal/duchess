#ifndef DUCHESS_CPP_TESTBRAIN_H
#define DUCHESS_CPP_TESTBRAIN_H

class TestBrain {
public:
    static void runTests();
private:
    static void test_settersAndGetters();
    static void test_sigmoid();
    static void test_evaluateBoardState();
    static void test_chanceOfWinningForBoard();
    static void test_saveLoadWeights();
    static void test_boardToFeatures();
    static void test_updateWeightsForFeatures();
    static void test_randomNumber();
    static void test_sortMovesBasedOnHeuristic();
    static void test_getPlayerOffsetBySeats();
    static void test_seatsBetweenPlayers();
    static void test_positionIndexesRelativeToPlayerLookup();
    static void test_naiveAttDef();
    static void test_attDef();
    static void test_pieceSquare();
    static void test_addTargetValToWeightDelta();
    static void test_kingDist();
};

#endif //DUCHESS_CPP_TESTBRAIN_H
