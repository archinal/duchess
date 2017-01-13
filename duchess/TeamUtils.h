#ifndef DUCHESS_TEAMUTILS_H
#define DUCHESS_TEAMUTILS_H

#include "TeamType.h"

#include <string>

class TeamUtils {
public:
    static TeamType getTeamForPlayer(const short t_player);
    static bool isSameTeam(const short t_player1, const short t_player2);
    static std::string teamToString(const TeamType type);
};

#endif //DUCHESS_TEAMUTILS_H
