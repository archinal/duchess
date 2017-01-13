#include "testSearchAnalytics.h"
#include "../SearchAnalytics.h"

#include <vector>
#include <cassert>
#include <chrono>

void TestSearchAnalytics::runTests() {
    TestSearchAnalytics::test_cutoffs();
    TestSearchAnalytics::test_expansions();
    TestSearchAnalytics::test_searchTime();
    TestSearchAnalytics::test_lowestDepth();
    TestSearchAnalytics::test_turnConversion();
    TestSearchAnalytics::test_piecesOnBoard();
    TestSearchAnalytics::test_reset();
}

void TestSearchAnalytics::test_cutoffs() {
    SearchAnalytics analytics(1, 90);

    assert(analytics.getCutoffsAtDepth(0) == 0);
    assert(analytics.getCutoffsAtDepth(5) == 0);

    analytics.recordCutoffAtDepth(5);
    analytics.recordCutoffAtDepth(5);

    assert(analytics.getCutoffsAtDepth(0) == 0);
    assert(analytics.getCutoffsAtDepth(5) == 2);

    // Make sure giving it an unexpectedly huge depth is okay
    analytics.recordCutoffAtDepth(500);

}

void TestSearchAnalytics::test_expansions() {
    SearchAnalytics analytics(1, 90);

    assert(analytics.getNodesExpandedAtDepth(1) == 0);
    assert(analytics.getNodesExpandedAtDepth(4) == 0);

    analytics.recordNodesExpandedAtDepth(4, 40);
    analytics.recordNodesExpandedAtDepth(4, 30);

    assert(analytics.getNodesExpandedAtDepth(1) == 0);
    assert(analytics.getNodesExpandedAtDepth(4) == 70);

    // Make sure giving it an unexpectedly huge depth is okay
    analytics.recordNodesExpandedAtDepth(500, 20);
}

void TestSearchAnalytics::test_searchTime() {
    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

    SearchAnalytics analytics(1, 90);
    analytics.startTiming();
    analytics.recordNodesExpandedAtDepth(4, 50);
    analytics.stopTiming();

    std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();

    assert(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() >= analytics.getDurationInMillis());
}

void TestSearchAnalytics::test_lowestDepth() {
    SearchAnalytics analytics(1, 90);
    assert(analytics.getLowestDepth() == 0);
    analytics.recordNodesExpandedAtDepth(4, 50);
    assert(analytics.getLowestDepth() == 4);
}

void TestSearchAnalytics::test_turnConversion() {
    SearchAnalytics analytics(5, 90);

    assert(analytics.getNodesExpandedAtDepth(1) == 0);
    assert(analytics.getNodesExpandedAtDepth(4) == 0);

    analytics.recordNodesExpandedAtTurn(9, 40);
    analytics.recordNodesExpandedAtTurn(9, 30);

    assert(analytics.getNodesExpandedAtDepth(1) == 0);
    assert(analytics.getNodesExpandedAtDepth(4) == 70);

}

void TestSearchAnalytics::test_reset() {
    SearchAnalytics analytics(1, 90);

    assert(analytics.getCutoffsAtDepth(0) == 0);
    assert(analytics.getCutoffsAtDepth(5) == 0);
    assert(analytics.getNodesExpandedAtDepth(1) == 0);
    assert(analytics.getNodesExpandedAtDepth(4) == 0);

    analytics.recordNodesExpandedAtDepth(4, 40);
    analytics.recordNodesExpandedAtDepth(4, 30);
    analytics.recordCutoffAtDepth(5);
    analytics.recordCutoffAtDepth(5);

    assert(analytics.getCutoffsAtDepth(0) == 0);
    assert(analytics.getCutoffsAtDepth(5) == 2);
    assert(analytics.getNodesExpandedAtDepth(1) == 0);
    assert(analytics.getNodesExpandedAtDepth(4) == 70);

    analytics.reset();

    assert(analytics.getCutoffsAtDepth(0) == 0);
    assert(analytics.getCutoffsAtDepth(5) == 0);
    assert(analytics.getNodesExpandedAtDepth(1) == 0);
    assert(analytics.getNodesExpandedAtDepth(4) == 0);
}

void TestSearchAnalytics::test_piecesOnBoard() {
    SearchAnalytics analytics(1, 90);
    assert(analytics.getNumPiecesOnBoard() == 90);
    SearchAnalytics analytics2(1);
    assert(analytics2.getNumPiecesOnBoard() == 0);
}