#include "GameLog.h"

#include "../duchess/Move.h"
#include "../duchess/TeamUtils.h"

#include <vector>
#include <chrono>
#include <string>
#include <sstream>

int GameLog::getNumTurns() const {
    return m_turnStrings.size();
}

void GameLog::recordMove(const Move& m) {
    m_turnStrings.push_back(m.toShortString());
}

std::string GameLog::getWinnerAsString() const {
    return TeamUtils::teamToString(m_winner);
}

void GameLog::startTiming() {
    m_startTime = std::chrono::high_resolution_clock::now();
}

void GameLog::stopTiming() {
    m_endTime = std::chrono::high_resolution_clock::now();
}

int GameLog::getDurationInSeconds() const {
    return std::chrono::duration_cast<std::chrono::seconds>(m_endTime - m_startTime).count();
}

std::string GameLog::toString() const {
    std::stringstream result;

    // Game ID, then result, then duration, then num moves
    result << "Game ID:," << this->getID() << std::endl;
    result << "Winner:," << this->getWinnerAsString() << std::endl;
    result << "Duration:," << this->getDurationInSeconds() << " seconds" << std::endl;
    result << "Transcript (" << this->getNumTurns() << " moves): " << std::endl;

    int i;
    for (i = 1; i <= 6; ++i) {
        result << "Player " << i;
        if (i != 6) {
            result << ", ";
        } else {
            result << std::endl;
        }
    }
    for (i = 0; i < this->getNumTurns(); ++i) {
        result << m_turnStrings[i];
        if (i % 6 != 5) {
            result << ", ";
        } else {
            result << std::endl;
        }
    }


    return result.str();
}