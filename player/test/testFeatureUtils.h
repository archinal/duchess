#ifndef DUCHESS_CPP_TESTFEATUREUTILS_H
#define DUCHESS_CPP_TESTFEATUREUTILS_H

class TestFeatureUtils {
public:
    static void runTests();
private:
    static void test_pieceTypeToIndex();
    static void test_doesFeatureSetContainFeatureType();
    static void test_getNumFeaturesAssociatedWithFeatureSet();

};

#endif //DUCHESS_CPP_TESTFEATUREUTILS_H
