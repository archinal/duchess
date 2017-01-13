#ifndef DUCHESS_CPP_HEURISTICCACHE_H
#define DUCHESS_CPP_HEURISTICCACHE_H

#include "../duchess/Move.h"

#include <map>
#include <set>
#include <string>

class HeuristicCache {
public:
    HeuristicCache();

    // For killer move
    void markMoveAsKiller(const Move t_move, const short t_player);
    void eraseMoveAsKiller(const Move t_move, const short t_player);
    std::set<Move> getKillerMoves(const short t_player) const;

    // For history
    void markMoveAsHistory(const Move t_move, const int& t_depth, const short t_player);
    void eraseMoveAsHistory(const Move t_move, const short t_player);
    std::vector<Move> getHistoryMoves(const short t_player) const;

    // Best reply
    void markMoveAsBestReply(const Move t_move, const Move& t_parent);
    void eraseMoveAsBestReply(const Move& t_parent);
    Move getBestReply(const Move& t_parent);
    bool hasBestReply(const Move& t_parent) const;

    bool isMoveKiller(const Move& t_move, const short t_player) const;
    int getHistoricalCutoffs(const Move t_move, const short t_player);
private:
    std::vector<std::set<Move>> m_killerMoves;
    std::vector<std::map<Move, unsigned int>> m_historyMoves;
    std::map<std::string, Move> m_bestReplies;
};

#endif //DUCHESS_CPP_HEURISTICCACHE_H
