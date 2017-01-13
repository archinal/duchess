#ifndef DUCHESS_CPP_GAMESETLOG_H
#define DUCHESS_CPP_GAMESETLOG_H

#include "../player/FeatureSet.h"
#include "../player/Heuristic.h"
#include "../duchess/EvaluationMode.h"
#include "GameLog.h"

#include <vector>
#include <string>

class GameSetLog {
public:
    GameSetLog(std::string t_logDest = "") : m_logDest(t_logDest) {}
    void setLogDest(std::string t_logDest) {
        m_logDest = t_logDest;
    }

    std::string getLogDest() const {
        return m_logDest;
    }

    static std::string getHeader();

    void addLog(const GameLog& t_log, const FeatureSet t_featureSet, const Heuristic t_heuristic, const EvaluationMode t_evaluationMode);
    void saveLogs() const;

private:
    std::string m_logDest;
    std::vector<std::string> m_logEntries;
};

#endif //DUCHESS_CPP_GAMESETLOG_H
