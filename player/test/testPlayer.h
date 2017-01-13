#ifndef DUCHESS_CPP_TESTPLAYER_H
#define DUCHESS_CPP_TESTPLAYER_H

class TestPlayer {
public:
    static void runTests();
private:
    static void test_playerGetterMethods();
    static void test_AIReturnsAMove();
    static void test_alphaBetaMakesSmartMoves();
};

#endif //DUCHESS_CPP_TESTPLAYER_H
