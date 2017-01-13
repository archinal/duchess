#ifndef DUCHESS_CPP_BRAIN_H
#define DUCHESS_CPP_BRAIN_H

#include <string>
#include <vector>
#include <ctime>

#include "Heuristic.h"
#include "HeuristicCache.h"
#include "FeatureSet.h"

#include "../duchess/Board.h"
#include "../duchess/Move.h"
#include "../duchess/MoveVector.h"
#include "../duchess/EvaluationMode.h"

typedef char Feature; // Feature just holds how many times this occurs

class Brain {
public:
    Brain(const std::string t_pathToWeights = "", EvaluationMode t_evaluationMode = EvaluationMode::CUMULATIVE) :
            m_pathToWeights(t_pathToWeights), m_weightsSavePath(""), m_logDestination(""), m_heuristic(Heuristic::NONE),
            m_featureSet(FeatureSet::MATERIAL), m_evaluationMode(t_evaluationMode), m_isLearning(false) {
        this->loadWeights();
        std::srand(std::time(0));
    }

    void setPathToWeights(const std::string t_pathToWeights) {
        m_pathToWeights = t_pathToWeights;
    }

    std::string getPathToWeights() const {
        return m_pathToWeights;
    }

    void setWeightsSavePath(const std::string t_weightsSavePath) {
        m_weightsSavePath = t_weightsSavePath;
    }

    std::string getWeightsSavePath() const {
        return m_weightsSavePath;
    }

    void setHeuristic(const Heuristic t_heuristic) {
        m_heuristic = t_heuristic;
    }

    Heuristic getHeuristic() const {
        return m_heuristic;
    }

    void setFeatureSet(const FeatureSet t_featureSet) {
        m_featureSet = t_featureSet;
    }

    FeatureSet getFeatureSet() const {
        return m_featureSet;
    }

    EvaluationMode getEvaluationMode() const {
        return this->m_evaluationMode;
    }

    void setEvaluationMode(EvaluationMode mode) {
        this->m_evaluationMode = mode;
    }

    HeuristicCache getHeuristicCache() const {
        return m_heuristicCache;
    }

    void sortMovesBasedOnHeuristic(std::vector<Move>& t_moves) const;
    double getValueOfPiece(const Piece* t_piece) const;

    double chanceOfWinningForBoard(const Board& t_board, const short t_player) const;
    double evaluateBoardState(const Board& t_board, const short t_player) const;
    double sigmoid(const double x) const;
    double multiplyFeaturesByWeights(const std::vector<Feature>& features, const std::vector<double>& weights, const short t_player, const Board& t_board) const;

    void loadWeights();
    void loadWeightsAndFillVoid();
    void saveWeights() const;
    void setWeights(std::vector<double> t_weights) {
        m_weights = t_weights;
    }
    std::string weightsToString() const;

    const std::vector<double>& getWeights() const {
        return m_weights;
    }

    std::vector<double>& getWeights() {
        return m_weights;
    }

    const std::vector<double>& getWeightDelta() const {
        return m_weightDelta;
    }

    std::vector<double>& getWeightDelta() {
        return m_weightDelta;
    }

    void resetWeightDelta();
    void applyWeightDelta();

    std::vector<Feature> boardToFeatures(const Board& t_board, const short t_player) const;
    void fillMaterialFeatures(std::vector<Feature>& t_features, const Board& t_board, const short t_player) const;
    void fillPartnerWizardFeatures(std::vector<Feature>& t_features, const Board& t_board, const short t_player) const;
    void fillPawnDistanceFeatures(std::vector<Feature>& t_features, const Board& t_board, const short t_player) const;
    void fillBiasFeatures(std::vector<Feature>& t_features) const;
    void fillCheckFeatures(std::vector<Feature>& t_features, const Board& t_board, const short t_player) const;
    void fillCheckmateFeatures(std::vector<Feature>& t_features, const Board& t_board, const short t_player) const;
    void fillAttackingAndDefendingFeatures(std::vector<Feature>& t_features, const Board& t_board, const short t_player) const;
    void fillAttackingAndDefendingFeaturesNaive(std::vector<Feature>& t_features, const Board& t_board, const short t_player) const;
    void fillKingDistFeatures(std::vector<Feature>& t_features, const Board& t_board, const short t_player) const;
    void fillPieceSquareFeatures(std::vector<Feature>& t_features, const Board& t_board, const short t_player) const;

    void updateWeightsForFeatures(const std::vector<Feature>& t_features, const double t_lowerBound, const double t_upperBound, const int t_depthInTree, const short t_player, const Board& t_board);
    void addTargetValToWeightDelta(const std::vector<Feature>& t_features, const double t_targetVal, const short t_player, const Board& t_board);
    double getDiscountFactorAtDepth(const int t_depth) const;

    int getRandomNumber(const int limit) const;

    void setLogDestination(std::string t_logDestination) {
        m_logDestination = t_logDestination;
    }

    std::string getLogDestination() const {
        return m_logDestination;
    }

    void addLineToSearchLogs(const std::string t_logLine);
    void writeLogs();

    bool isLearning() const {
        return this->m_isLearning;
    }

    void setLearning(const bool t_learning) {
        m_isLearning = t_learning;
    }

    static short getPlayerOffsetBySeats(const short t_basePlayer, const short t_seatsAround);
    static short getSeatsBetweenPlayers(const short t_playerA, const short t_playerB);
    static unsigned int moveVectorToAttIndex(const MoveVector* t_moveVector, const short t_player);
    static unsigned int moveVectorToNaiveAttIndex(const MoveVector* t_moveVector, const short t_player);
    static unsigned int moveVectorToDefIndex(const MoveVector* t_moveVector, const short t_player);
    static unsigned int moveVectorToNaiveDefIndex(const MoveVector* t_moveVector, const short t_player);
    static int getKingDistFeatureIndex(const PieceType t_pieceType, const short t_basePlayer, const short t_pieceOwner, const short t_kingOwner, const Position* t_piecePosition, const Position* t_kingPosition);


    constexpr static double LEARNING_RATE {1e-5};
    constexpr static double DISCOUNT_FACTOR {1/3};
    constexpr static double WINNING_MOVE_SCORE {1e5};
    constexpr static double MAX_WEIGHT_DELTA {0.001};
    // Accounts for floating point errors to encourage earlier cutoffs
    constexpr static double FLOAT_FUDGE {0}; // or 1e-8
    constexpr static int MAX_SEARCH_TIME_MILLIS {1000};

    // A lookup table of position index relative to a player
    // Helps us determine the order in which to evaluate features for piece-square
    static const BoardIndex positionIndexesRelativeToPlayer[6][6 * 157];

private:
    std::string m_pathToWeights;
    std::string m_weightsSavePath;
    std::string m_logDestination;

    // The heuristic to use for board evaluations
    // Default: NONE
    Heuristic m_heuristic;
    // The feature set to use
    // Default: MATERIAL
    FeatureSet m_featureSet;
    EvaluationMode m_evaluationMode;
    bool m_isLearning;

    // The weights that we apply to different features
    std::vector<double> m_weights;

    // Some "interim weights" that are generated by the learning algorithm
    // These can later be added into m_weights via applyWeightDelta()
    // or reset via resetWeightDelta()
    std::vector<double> m_weightDelta;

    std::vector<std::string> m_searchAnalyticsLogs;

    HeuristicCache m_heuristicCache;

    static const double discountRateAtDepth[15];

};

#endif //DUCHESS_CPP_BRAIN_H
