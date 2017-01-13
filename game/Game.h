#ifndef DUCHESS_CPP_GAME_H
#define DUCHESS_CPP_GAME_H

#include "GameLog.h"

#include "../player/Player.h"

#include "../duchess/Board.h"
#include "../duchess/Move.h"
#include "../duchess/EvaluationMode.h"

#include <vector>
#include <string>

class Game {
public:
    Game(const std::vector<Player*> t_players, const EvaluationMode t_evaluationMode);

    std::vector<Player*> getPlayers() const {
        return this->m_players;
    }

    Board& getBoard() {
        return this->m_board;
    }

    EvaluationMode getEvaluationMode() const {
        return this->m_evaluationMode;
    };

    void run();

    std::string getID() const {
        return m_id;
    }

    GameLog& getLog() {
        return m_log;
    }

    std::string getLogLocation() const {
        return m_logLocation;
    }

    void setLogLocation(const std::string t_location) {
        m_logLocation = t_location;
    }

    void saveLog() const;

private:
    const std::vector<Player*> m_players;
    const EvaluationMode m_evaluationMode;
    Board m_board;
    const int MAX_TURNS {660};
    GameLog m_log;
    std::string m_id;
    std::string m_logLocation;
};

#endif //DUCHESS_CPP_GAME_H
