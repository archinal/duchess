#ifndef DUCHESS_CPP_TESTGAMELOG_H
#define DUCHESS_CPP_TESTGAMELOG_H

class TestGameLog {
public:
    static void runTests();
private:
    static void test_getters();
    static void test_addingMoves();
    static void test_settingWinner();
    static void test_duration();
};

#endif //DUCHESS_CPP_TESTGAMELOG_H
