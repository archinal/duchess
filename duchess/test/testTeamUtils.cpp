#include "testTeamUtils.h"
#include "../TeamType.h"
#include "../TeamUtils.h"

#include <cassert>

void TestTeamUtils::runTests() {
    TestTeamUtils::testGetTeamForPlayer();
    TestTeamUtils::testIsSameTeam();
}

void TestTeamUtils::testGetTeamForPlayer() {
    assert(TeamUtils::getTeamForPlayer(1) == TeamType::ODDS);
    assert(TeamUtils::getTeamForPlayer(3) == TeamType::ODDS);
    assert(TeamUtils::getTeamForPlayer(5) == TeamType::ODDS);
    assert(TeamUtils::getTeamForPlayer(2) == TeamType::EVENS);
    assert(TeamUtils::getTeamForPlayer(4) == TeamType::EVENS);
    assert(TeamUtils::getTeamForPlayer(6) == TeamType::EVENS);
}

void TestTeamUtils::testIsSameTeam() {
    assert(TeamUtils::isSameTeam(1, 1));
    assert(TeamUtils::isSameTeam(1, 3));
    assert(TeamUtils::isSameTeam(1, 5));
    assert(TeamUtils::isSameTeam(3, 5));

    assert(TeamUtils::isSameTeam(2, 2));
    assert(TeamUtils::isSameTeam(2, 4));
    assert(TeamUtils::isSameTeam(2, 6));
    assert(TeamUtils::isSameTeam(4, 6));

    assert(!TeamUtils::isSameTeam(1, 6));
    assert(!TeamUtils::isSameTeam(1, 4));
    assert(!TeamUtils::isSameTeam(1, 2));
    assert(!TeamUtils::isSameTeam(2, 1));
    assert(!TeamUtils::isSameTeam(2, 5));
    assert(!TeamUtils::isSameTeam(6, 5));
}
