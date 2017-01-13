#include "testGameLog.h"

#include "../GameLog.h"
#include "../../duchess/Move.h"
#include "../../duchess/TeamType.h"

#include <cassert>
#include <string>

void TestGameLog::runTests() {
    TestGameLog::test_getters();
    TestGameLog::test_addingMoves();
    TestGameLog::test_settingWinner();
    TestGameLog::test_duration();
}

void TestGameLog::test_getters() {
    const std::string id = "the_id";
    GameLog g (id);
    assert(g.getID() == id);
}

void TestGameLog::test_addingMoves() {
    GameLog g ("id");
    assert(g.getNumTurns() == 0);
    Move m;
    g.recordMove(m);
    assert(g.getNumTurns() == 1);
}

void TestGameLog::test_settingWinner() {
    GameLog g ("id");
    assert(g.getWinner() == TeamType::NONE);
    assert(g.getWinnerAsString() == "NONE");

    g.setWinner(TeamType::EVENS);
    assert(g.getWinner() == TeamType::EVENS);
    assert(g.getWinnerAsString() == "EVENS");

    g.setWinner(TeamType::ODDS);
    assert(g.getWinner() == TeamType::ODDS);
    assert(g.getWinnerAsString() == "ODDS");
}

void TestGameLog::test_duration() {
    GameLog g ("id");
    g.startTiming();
    g.stopTiming();
    assert(g.getDurationInSeconds() >= 0);
}