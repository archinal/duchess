#ifndef DUCHESS_TESTPOSITION_H
#define DUCHESS_TESTPOSITION_H

class TestPosition {
public:
    static void runTests();

private:
    static void test_accessorsRevealData();
    static void test_toStringConvertsToString();
    static void test_equalityOperatorWorks();
    static void test_constructors();
    static void test_sanitizedConstructor();
};

#endif //DUCHESS_TESTPOSITION_H
