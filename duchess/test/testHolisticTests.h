#ifndef DUCHESS_CPP_TESTHOLISTICTESTS_H
#define DUCHESS_CPP_TESTHOLISTICTESTS_H

#include "../Board.h"
#include "../Move.h"

class TestHolisticTests {
public:
    static void runTests();
private:
    static void test_linesThroughVortex();
    static void test_kingOnVortex();
    static void applyMoveAndUndoCheckingAllSame(Board& board, const Move& move);

};

#endif //DUCHESS_CPP_TESTHOLISTICTESTS_H
