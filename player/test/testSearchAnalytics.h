#ifndef DUCHESS_CPP_TESTSEARCHANALYTICS_H
#define DUCHESS_CPP_TESTSEARCHANALYTICS_H

class TestSearchAnalytics {
public:
    static void runTests();
private:
    static void test_cutoffs();
    static void test_expansions();
    static void test_searchTime();
    static void test_lowestDepth();
    static void test_turnConversion();
    static void test_piecesOnBoard();
    static void test_reset();
};

#endif //DUCHESS_CPP_TESTSEARCHANALYTICS_H
