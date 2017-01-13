#ifndef DUCHESS_CPP_PLAYER_H
#define DUCHESS_CPP_PLAYER_H

#include "PlayerType.h"
#include "Brain.h"
#include "SearchAnalytics.h"

#include "../game/GameResult.h"

#include "../duchess/Board.h"
#include "../duchess/Move.h"

class Player {
public:
    Player(const PlayerType t_playerType, short t_playerNumber) :
            m_playerType(t_playerType), m_playerNumber(t_playerNumber), m_stochastic(false)
            {}

    Move chooseMove(Board* t_board, const int t_globalTurn);
    Move searchForMove(Board* t_board, const std::vector<Move> t_validMoves, const int t_globalTurn);
    Move alphaBetaFromRoot(std::vector<Move> t_validMoves, Board* t_board, const int t_maxDepth, SearchAnalytics* searchAnalytics, const int t_globalTurn) const;
    double alphaBetaMoveSearch(Board* t_board, const int t_depth, double t_alpha, double t_beta, const short t_turn, SearchAnalytics* searchAnalytics, const int t_globalTurn, const Move& t_parentMove, const int t_maxDepth) const;
    void notifyResult(const GameResult t_result);

    PlayerType getPlayerType() const {
        return this->m_playerType;
    }

    void setPlayerNumber(short num) {
        this->m_playerNumber = num;
    }

    short getPlayerNumber() const {
        return this->m_playerNumber;
    }

    void setBrain(Brain* t_brain) {
        this->m_brain = t_brain;
    }

    Brain* getBrain() const {
        return this->m_brain;
    }

    bool isStochastic() const {
        return m_stochastic;
    }

    void setStochastic(const bool stochastic) {
        m_stochastic = stochastic;
    }

    // AI will do random moves for the first few moves
    constexpr static int AI_RANDOM_MOVE_THRESHOLD {6};
    constexpr static double RANDOM_MOVE_CHANCE {0.05};

private:
    Move chooseMoveAsHuman(Board* t_board, const std::vector<Move>* t_moves) const;

    const PlayerType m_playerType;
    short m_playerNumber;
    bool m_stochastic;


    // If we're an AI, we need a brain to store things like paths to weights files
    Brain* m_brain;
};

#endif //DUCHESS_CPP_PLAYER_H
