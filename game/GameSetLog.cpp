#include "GameSetLog.h"
#include "../player/FeatureSet.h"
#include "../player/Heuristic.h"
#include "../duchess/EvaluationMode.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

void GameSetLog::addLog(const GameLog& t_log, const FeatureSet t_featureSet, const Heuristic t_heuristic, const EvaluationMode t_evaluationMode) {
    std::stringstream line;

    std::string featureSetStr;
    std::string heuristicStr;
    std::string evaluationModeStr;

    if (t_featureSet == FeatureSet::MATERIAL) {
        featureSetStr = "Material/Check/PawnDist";
    } else if (t_featureSet == FeatureSet::MATERIAL_ATTDEF) {
        featureSetStr = "Material/Check/PawnDist/AttDef";
    } else if (t_featureSet == FeatureSet::MATERIAL_ATTDEF_PIECESQUARE) {
        featureSetStr = "Material/Check/AttDef/PieceSquare";
    } else if (t_featureSet == FeatureSet::MATERIAL_NAIVEATTDEF) {
        featureSetStr = "Material/Check/NaiveAttDef";
    } else if (t_featureSet == FeatureSet::MATERIAL_NAIVEATTDEF_PIECESQUARE) {
        featureSetStr = "Material/Check/NaiveAttDef/PieceSquare";
    } else if (t_featureSet == FeatureSet::MATERIAL_ATTDEF_KINGDIST) {
        featureSetStr = "Material/Check/AttDef/KingDist";
    } else if (t_featureSet == FeatureSet::MATERIAL_ATTDEF_KINGDIST_PIECE_SQUARE) {
        featureSetStr = "Material/Check/AttDef/KingDist/PieceSquare";
    } else {
        featureSetStr = "All";
    }

    if (t_heuristic == Heuristic::KILLER_MOVE) {
        heuristicStr = "Killer Move";
    } else if (t_heuristic == Heuristic::BEST_REPLY) {
        heuristicStr = "Best Reply";
    } else if (t_heuristic == Heuristic::HISTORY) {
        heuristicStr = "History";
    } else {
        heuristicStr = "None";
    }

    if (t_evaluationMode == EvaluationMode::CUMULATIVE) {
        evaluationModeStr = "Cumulative";
    } else {
        evaluationModeStr = "DeNovo";
    }

    line << t_log.getID() << ", ";
    line << t_log.getWinnerAsString() << ", ";
    line << t_log.getDurationInSeconds() << ", ";
    line << featureSetStr << ", ";
    line << heuristicStr << ", ";
    line << evaluationModeStr << ", ";

    line << t_log.getNumTurns();

    m_logEntries.push_back(line.str());

}

void GameSetLog::saveLogs() const {
    const std::string dest = this->getLogDest();
    if (dest != "") {
        std::ofstream outFile(dest);
        // Write CSV Header
        outFile << GameSetLog::getHeader() << std::endl;

        // Write values
        for (auto const& logLine : m_logEntries) {
            outFile << logLine << std::endl;
        }
        outFile.close();
    }
}

std::string GameSetLog::getHeader() {
    std::stringstream result;
    result << "Game ID, ";
    result << "Winner, ";
    result << "Duration, ";
    result << "Feature Set, ";
    result << "Heuristic, ";
    result << "Evaluation Mode, ";
    result << "Number of Turns";
    return result.str();

}