#include "SearchAnalytics.h"

#include <vector>
#include <chrono>
#include <sstream>

SearchAnalytics::SearchAnalytics(const int t_turnNumber, const int t_numPiecesOnBoard) :
        m_turnNumber(t_turnNumber), m_numPiecesOnBoard(t_numPiecesOnBoard), m_lowestDepth(0) {
    m_cutoffsAtDepth.reserve(SearchAnalytics::MAX_DEPTH_RECORD);
    m_nodesExpandedAtDepth.reserve(SearchAnalytics::MAX_DEPTH_RECORD);
    m_nodesEvaluatedAtDepth.reserve(SearchAnalytics::MAX_DEPTH_RECORD);

    m_cutoffsAtDepth.assign(MAX_DEPTH_RECORD, 0);
    m_nodesExpandedAtDepth.assign(MAX_DEPTH_RECORD, 0);
    m_nodesEvaluatedAtDepth.assign(MAX_DEPTH_RECORD, 0);

}

void SearchAnalytics::startTiming() {
    m_startTime = std::chrono::high_resolution_clock::now();
}

void SearchAnalytics::stopTiming() {
    m_endTime = std::chrono::high_resolution_clock::now();
}

int SearchAnalytics::getDurationInMillis() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(m_endTime - m_startTime).count();
}

void SearchAnalytics::recordCutoffAtDepth(const int t_depth) {
    if (t_depth < SearchAnalytics::MAX_DEPTH_RECORD) {
        m_cutoffsAtDepth[t_depth]++;
    }
}

void SearchAnalytics::recordNodesExpandedAtDepth(const int t_depth, const int t_numNodes) {
    if (t_depth < SearchAnalytics::MAX_DEPTH_RECORD) {
        m_nodesExpandedAtDepth[t_depth] += t_numNodes;
    }

    if (t_depth > m_lowestDepth) {
        m_lowestDepth = t_depth;
    }
}

void SearchAnalytics::recordNodesEvaluatedAtDepth(const int t_depth, const int t_numNodes) {
    if (t_depth < SearchAnalytics::MAX_DEPTH_RECORD) {
        m_nodesEvaluatedAtDepth[t_depth] += t_numNodes;
    }

    if (t_depth > m_lowestDepth) {
        m_lowestDepth = t_depth;
    }
}

void SearchAnalytics::recordCutoffAtTurn(const int t_globalTurn) {
    this->recordCutoffAtDepth(t_globalTurn - m_turnNumber);
}

void SearchAnalytics::recordNodesExpandedAtTurn(const int t_globalTurn, const int t_numNodes) {
    this->recordNodesExpandedAtDepth(t_globalTurn - m_turnNumber, t_numNodes);
}

void SearchAnalytics::recordNodesEvaluatedAtTurn(const int t_globalTurn, const int t_numNodes) {
    this->recordNodesEvaluatedAtDepth(t_globalTurn - m_turnNumber, t_numNodes);
}


void SearchAnalytics::reset() {
    m_cutoffsAtDepth.assign(MAX_DEPTH_RECORD, 0);
    m_nodesExpandedAtDepth.assign(MAX_DEPTH_RECORD, 0);
    m_nodesEvaluatedAtDepth.assign(MAX_DEPTH_RECORD, 0);
}

std::string SearchAnalytics::toString() const {
    std::stringstream result;

    // TURN_NUMBER, MAX_DEPTH, DURATION, forall cutoffs(CUTOFF), forall nodes (NODES)

    result << this->getTurnNumber() << ", ";
    result << this->getNumPiecesOnBoard() << ", ";
    result << this->getLowestDepth() << ", ";
    result << this->getDurationInMillis();

    for (auto const& cutoffCount : m_cutoffsAtDepth) {
        result << ", " << cutoffCount;
    }

    for (auto const& nodeExpansionCount : m_nodesExpandedAtDepth) {
        result << ", " << nodeExpansionCount;
    }

    for (auto const& nodeEvaluationCount : m_nodesEvaluatedAtDepth) {
        result << ", " << nodeEvaluationCount;
    }

    return result.str();
}

std::string SearchAnalytics::toShortString() const {
    std::stringstream result;

    // TURN_NUMBER, MAX_DEPTH, DURATION

    result << "Turn: " << this->getTurnNumber() << ", ";
    result << "# Pieces: " << this->getNumPiecesOnBoard() << ", ";
    result << "Max depth: " << this->getLowestDepth() << ", ";
    result << "Duration: " << this->getDurationInMillis();

    return result.str();
}

std::string SearchAnalytics::getHeader() {
    std::stringstream result;
    // TURN_NUMBER, MAX_DEPTH, DURATION, forall cutoffs(CUTOFF), forall nodes (NODES)
    result << "Turn number, ";
    result << "Pieces on board, ";
    result << "Maximum depth, ";
    result << "Duration (millis)";

    int i;
    for (i = 0; i < SearchAnalytics::MAX_DEPTH_RECORD; ++i) {
        result << ", Cutoffs at depth " << i;
    }

    for (i = 0; i < SearchAnalytics::MAX_DEPTH_RECORD; ++i) {
        result << ", Nodes expanded at depth " << i;
    }

    for (i = 0; i < SearchAnalytics::MAX_DEPTH_RECORD; ++i) {
        result << ", Nodes evaluated at depth " << i;
    }

    return result.str();
}