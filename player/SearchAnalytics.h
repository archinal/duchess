#ifndef DUCHESS_CPP_SEARCHANALYTICS_H
#define DUCHESS_CPP_SEARCHANALYTICS_H

#include <string>
#include <vector>
#include <chrono>

class SearchAnalytics {
public:
    SearchAnalytics(const int t_turnNumber = 1, const int t_numPiecesOnBoard = 0);

    int getTurnNumber() const {
        return this->m_turnNumber;
    }

    void recordCutoffAtDepth(const int t_depth);
    void recordNodesExpandedAtDepth(const int t_depth, const int t_numNodes);
    void recordNodesEvaluatedAtDepth(const int t_depth, const int t_numNodes);

    // These functions exist because the ab search doesn't know its own depth (just how far to go)
    // It does know the global turn at the depth though, so we can use that
    void recordCutoffAtTurn(const int t_globalTurn);
    void recordNodesExpandedAtTurn(const int t_globalTurn, const int t_numNodes);
    void recordNodesEvaluatedAtTurn(const int t_globalTurn, const int t_numNodes);

    int getCutoffsAtDepth(const int t_depth) const {
        return m_cutoffsAtDepth[t_depth];
    }

    int getNodesExpandedAtDepth(const int t_depth) const {
        return m_nodesExpandedAtDepth[t_depth];
    }

    int getNodesEvaluatedAtDepth(const int t_depth) const {
        return m_nodesEvaluatedAtDepth[t_depth];
    }

    int getLowestDepth() const {
        return m_lowestDepth;
    }

    int getNumPiecesOnBoard() const {
        return m_numPiecesOnBoard;
    }

    void reset();
    void startTiming();
    void stopTiming();
    int getDurationInMillis() const;

    std::string toString() const;
    std::string toShortString() const;

    static std::string getHeader();

    constexpr static int MAX_DEPTH_RECORD {15};

private:
    const int m_turnNumber;
    const int m_numPiecesOnBoard;
    int m_lowestDepth;

    std::vector<int> m_cutoffsAtDepth;
    std::vector<int> m_nodesExpandedAtDepth;
    std::vector<int> m_nodesEvaluatedAtDepth;
    std::chrono::high_resolution_clock::time_point m_startTime;
    std::chrono::high_resolution_clock::time_point m_endTime;
};

#endif //DUCHESS_CPP_SEARCHANALYTICS_H
