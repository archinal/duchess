#ifndef DUCHESS_TESTPIECE_H
#define DUCHESS_TESTPIECE_H

class TestPiece {
public:
    static void runTests();

private:
    static void test_accessorsRevealData();
    static void test_constructors();
    static void test_updatePosition();
    static void test_equals();
    static void test_moveVectorManipulators();

};


#endif //DUCHESS_TESTPIECE_H
