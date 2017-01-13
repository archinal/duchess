#ifndef DUCHESS_TESTMOVE_H
#define DUCHESS_TESTMOVE_H

class TestMove {
public:
    static void runTests();

private:
    static void test_constructorsAndGetters();
    static void test_equals();
    static void test_lessThan();
};

#endif //DUCHESS_TESTMOVE_H
