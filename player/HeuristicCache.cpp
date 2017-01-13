#include "HeuristicCache.h"
#include "../duchess/Move.h"

#include <map>
#include <set>
#include <string>
#include <utility>
#include <algorithm>

HeuristicCache::HeuristicCache() {
    m_killerMoves.reserve(6);
    m_historyMoves.reserve(6);

    int i;
    for (i = 0; i < 6; ++i) {
        std::set<Move> moveSet;
        m_killerMoves.push_back(moveSet);

        std::map<Move, unsigned int> histories;
        m_historyMoves.push_back(histories);
    }
}

// For killer move
void HeuristicCache::markMoveAsKiller(const Move t_move, const short t_player) {
    if (!t_move.isNull()) {
        m_killerMoves[t_player - 1].insert(t_move);
    }
}

void HeuristicCache::eraseMoveAsKiller(const Move t_move, const short t_player) {
    m_killerMoves[t_player - 1].erase(t_move);
}

std::set<Move> HeuristicCache::getKillerMoves(const short t_player) const {
    return m_killerMoves[t_player - 1];
}


// For history
void HeuristicCache::markMoveAsHistory(const Move t_move, const int& t_depth, const short t_player) {
    const int val = t_depth * t_depth;
    if (m_historyMoves[t_player - 1].count(t_move) == 0) {
        m_historyMoves[t_player - 1][t_move] = val;
    } else {
        m_historyMoves[t_player - 1][t_move] += val;
    }
}

void HeuristicCache::eraseMoveAsHistory(const Move t_move, const short t_player) {
    m_historyMoves[t_player - 1].erase(t_move);
}

std::vector<Move> HeuristicCache::getHistoryMoves(const short t_player) const {
    std::vector<std::pair<Move, unsigned int>> movesToVal;
    std::vector<Move> moves;

    // Translate the hashmap into a vector of pairs sorted by the heuristic value
    for (auto it = m_historyMoves[t_player - 1].begin(); it != m_historyMoves[t_player - 1].end(); ++it) {
        movesToVal.push_back(*it);
    }


    std::sort(movesToVal.begin(), movesToVal.end(),
              [&] (const std::pair<Move, unsigned int>& a, const std::pair<Move, unsigned int>& b) -> bool {
                  return a.second > b.second;
              });

    // Fill moves
    moves.reserve(movesToVal.size());
    for (auto const& moveVal : movesToVal) {
        moves.push_back(moveVal.first);
    }

    return moves;
}


// Best reply
void HeuristicCache::markMoveAsBestReply(const Move t_move, const Move& t_parent) {
    m_bestReplies[t_parent.toShortString()] = t_move;
}

void HeuristicCache::eraseMoveAsBestReply(const Move& t_parent) {
    m_bestReplies.erase(t_parent.toShortString());
}

Move HeuristicCache::getBestReply(const Move& t_parent) {
    return m_bestReplies[t_parent.toShortString()];
}

bool HeuristicCache::hasBestReply(const Move& t_parent) const {
    return m_bestReplies.count(t_parent.toShortString()) == 1;
}

bool HeuristicCache::isMoveKiller(const Move& t_move, const short t_player) const {
    return m_killerMoves[t_player - 1].find(t_move) != m_killerMoves[t_player - 1].end();
}

int HeuristicCache::getHistoricalCutoffs(const Move t_move, const short t_player) {
    return m_historyMoves[t_player - 1].count(t_move) == 1 ? m_historyMoves[t_player - 1][t_move] : 0;

}