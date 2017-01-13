#include "TeamUtils.h"
#include "TeamType.h"
#include <string>

TeamType TeamUtils::getTeamForPlayer(const short t_player) {
    return (t_player % 2 == 1 ? TeamType::ODDS : TeamType::EVENS);
}

bool TeamUtils::isSameTeam(const short t_player1, const short t_player2) {
    return TeamUtils::getTeamForPlayer(t_player1) == TeamUtils::getTeamForPlayer(t_player2);
}

std::string TeamUtils::teamToString(const TeamType type) {
    std::string result = "NONE";
    if (type == TeamType::EVENS) {
        result = "EVENS";
    } else if (type == TeamType::ODDS) {
        result = "ODDS";
    }

    return result;
}