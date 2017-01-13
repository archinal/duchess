#include "testHeuristicCache.h"

#include "../../duchess/Move.h"
#include "../../duchess/Board.h"
#include "../HeuristicCache.h"

#include <string>
#include <vector>
#include <cassert>

void TestHeuristicCache::runTests() {
    TestHeuristicCache::test_history();
    TestHeuristicCache::test_bestReply();
    TestHeuristicCache::test_killer();
}

void TestHeuristicCache::test_history() {
    Board b;
    b.initialiseAttDefVectors();
    const std::vector<Move> moves = b.getLegalMoves(1);

    const Move m1 = moves[0];
    const Move m2 = moves[1];
    const Move m3 = moves[2];
    const Move m4 = moves[3];

    HeuristicCache cache;

    cache.markMoveAsHistory(m1, 3, 1);
    cache.markMoveAsHistory(m3, 5, 1);
    cache.markMoveAsHistory(m4, 4, 1);

    assert(cache.getHistoricalCutoffs(m1, 1) == 9);
    assert(cache.getHistoricalCutoffs(m1, 2) == 0);
    assert(cache.getHistoricalCutoffs(m2, 1) == 0);

    const std::vector<Move> expectedOrder {m3, m4, m1};
    assert(cache.getHistoryMoves(1) == expectedOrder);
    assert(cache.getHistoryMoves(6).size() == 0);

}

void TestHeuristicCache::test_bestReply() {
    Board b;
    b.initialiseAttDefVectors();
    const std::vector<Move> moves = b.getLegalMoves(1);

    const Move m1 = moves[0];
    const Move m2 = moves[1];
    const Move m3 = moves[2];

    HeuristicCache cache;

    cache.markMoveAsBestReply(m2, m1);
    assert(cache.getBestReply(m1) == m2);
    assert(!cache.hasBestReply(m3));
}

void TestHeuristicCache::test_killer() {
    Board b;
    b.initialiseAttDefVectors();
    const std::vector<Move> moves = b.getLegalMoves(6);

    const Move m1 = moves[0];
    const Move m2 = moves[1];
    const Move m3 = moves[2];

    HeuristicCache cache;

    cache.markMoveAsKiller(m1, 6);
    cache.markMoveAsKiller(m3, 6);

    assert(cache.isMoveKiller(m1, 6));
    assert(!cache.isMoveKiller(m2, 6));
    assert(cache.getKillerMoves(6).size() == 2);
    assert(cache.getKillerMoves(4).size() == 0);
}