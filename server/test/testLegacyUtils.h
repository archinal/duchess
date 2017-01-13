#ifndef DUCHESS_CPP_TESTLEGACYUTILS_H
#define DUCHESS_CPP_TESTLEGACYUTILS_H

class TestLegacyUtils {
public:
    static void runTests();
private:
    static void test_legacyPieceNumberToPieceIndex();
    static void test_legacyPieceNumberToOwner();
    static void test_legacySquareToPosition();
    static void test_positionToLegacyPosIndex();
};

#endif //DUCHESS_CPP_TESTLEGACYUTILS_H
