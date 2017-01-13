#ifndef DUCHESS_CPP_TESTMOVEVECTOR_H
#define DUCHESS_CPP_TESTMOVEVECTOR_H

class TestMoveVector {
public:
    static void runTests();

private:
    static void test_constructorAndAccessors();
    static void test_doesInvolve();
    static void test_doesCrossOver();
    static void test_registerDeregister();
};

#endif //DUCHESS_CPP_TESTMOVEVECTOR_H
