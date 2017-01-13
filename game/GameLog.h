#ifndef DUCHESS_CPP_GAMELOG_H
#define DUCHESS_CPP_GAMELOG_H

#include "../duchess/Move.h"
#include "../duchess/TeamType.h"

#include <string>
#include <vector>
#include <chrono>

class GameLog {
public:
    GameLog(std::string t_id = "") : m_id(t_id), m_winner(TeamType::NONE) {}

    void setID(const std::string t_id) {
        m_id = t_id;
    }

    std::string getID() const {
        return m_id;
    }

    void setWinner(const TeamType t_winner) {
        m_winner = t_winner;
    }

    TeamType getWinner() const {
        return m_winner;
    }

    int getNumTurns() const;
    std::string getWinnerAsString() const;
    void recordMove(const Move& m);
    void startTiming();
    void stopTiming();
    int getDurationInSeconds() const;

    std::string toString() const;

private:
    std::string m_id;
    TeamType m_winner;
    std::vector<std::string> m_turnStrings;

    std::chrono::high_resolution_clock::time_point m_startTime;
    std::chrono::high_resolution_clock::time_point m_endTime;

};

#endif //DUCHESS_CPP_GAMELOG_H
