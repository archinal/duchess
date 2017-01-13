#define NDEBUG

#include "Brain.h"
#include "FeatureUtils.h"
#include "FeatureSet.h"
#include "SearchAnalytics.h"

#include "../duchess/Board.h"
#include "../duchess/Piece.h"
#include "../duchess/MoveVector.h"
#include "../duchess/Position.h"
#include "../duchess/TeamUtils.h"
#include "../duchess/PieceType.h"

#include <algorithm>
#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include <ctime>

// A naive move ordering heuristic to prioritise moves that take other pieces
// Known as the Most-Valuable Victim heuristic
void Brain::sortMovesBasedOnHeuristic(std::vector<Move>& t_moves) const {
    std::sort(t_moves.begin(), t_moves.end(),
              [&] (const Move& a, const Move& b) -> bool {
                  return this->getValueOfPiece(a.getReplacePiece()) + this->getValueOfPiece(a.getPieceTaken()) > this->getValueOfPiece(b.getReplacePiece()) + this->getValueOfPiece(b.getPieceTaken());
              });
}

double Brain::getValueOfPiece(const Piece* t_piece) const {
    if (t_piece == nullptr) {
        return 0;
    } else {
        return this->getWeights()[FeatureUtils::pieceTypeToIndex(t_piece->getType())];
    }
}

// Only used in training
double Brain::chanceOfWinningForBoard(const Board& t_board, const short t_player) const {
    return this->sigmoid(this->evaluateBoardState(t_board, t_player));
}

// For use in AB Search if NOT training
double Brain::evaluateBoardState(const Board& t_board, const short t_player) const {
    return this->multiplyFeaturesByWeights(this->boardToFeatures(t_board, t_player), this->getWeights(), t_player, t_board);
}

double Brain::multiplyFeaturesByWeights(const std::vector<Feature>& t_features, const std::vector<double>& t_weights, const short t_player, const Board& t_board) const {
    // Check that they have the same length
    assert(t_features.size() == t_weights.size());

    // Multiply them together (dot product)
    double dotProduct = 0;

    const FeatureSet featureSet = this->getFeatureSet();

    // Handle Feature Types associated with FeatureSet::MATERIAL
    unsigned int i;
    for (i = FeatureUtils::INDEX_MATERIAL_WEIGHTS_START; i < FeatureUtils::INDEX_ATTACKING_WEIGHTS_START; ++i) {
        if (t_features[i] != 0) {
            dotProduct += t_features[i] * t_weights[i];
        }
    }

    if (FeatureUtils::doesFeatureSetContainFeatureType(featureSet, FeatureType::ATTACKING_DEFENDING_NAIVE)) {
        // We're using naive att def weights, so it's more efficient just to iterate along them too
        for (i = FeatureUtils::INDEX_ATTACKING_NAIVE_WEIGHTS_START; i < FeatureUtils::INDEX_PIECE_SQUARE_AFTER_NAIVE_WEIGHTS_START; ++i) {
            if (t_features[i] != 0) {
                dotProduct += t_features[i] * t_weights[i];
            }
        }
    } else if (FeatureUtils::doesFeatureSetContainFeatureType(featureSet, FeatureType::ATTACKING_DEFENDING)) {
        // Add for all active att/def features
        short player;
        for (player = 1; player <= FeatureUtils::NUM_PLAYERS; ++player) {
            for (auto const& piece : t_board.getPiecesForPlayer(player)) {
                for (auto const& mv : piece->getActiveAttackingVectors()) {
                    const unsigned int idx = Brain::moveVectorToAttIndex(mv, t_player);
                    dotProduct += t_features[idx] * t_weights[idx];
                }
                for (auto const& mv : piece->getActiveDefendingVectors()) {
                    const unsigned int idx = Brain::moveVectorToDefIndex(mv, t_player);
                    dotProduct += t_features[idx] * t_weights[idx];
                }
            }
        }
    }

    if (FeatureUtils::doesFeatureSetContainFeatureType(featureSet, FeatureType::KING_DISTANCE)) {
        short playerOffset;
        for (playerOffset = 0; playerOffset < FeatureUtils::NUM_PLAYERS; ++playerOffset) {
            const short player = Brain::getPlayerOffsetBySeats(t_player, playerOffset);
            // Get enemy kings relative to this player
            std::vector<Piece*> enemyKings;
            short offset;
            for (offset = 1; offset <= 5; offset += 2) {
                enemyKings.push_back(t_board.getPiecesForPlayer(Brain::getPlayerOffsetBySeats(player, offset))[12]);
            }
            // For each of this player's pieces, inc the feature index associated with that piece being DISTANCE from the enemy king
            for (auto const& piece : t_board.getPiecesForPlayer(player)) {
                if (!piece->getPosition()->isOffBoard()) {
                    for (auto const& enemyKing : enemyKings) {
                        const int idx = Brain::getKingDistFeatureIndex(piece->getType(), t_player, player, enemyKing->getOwner(), piece->getPosition(), enemyKing->getPosition());
                        if (idx >= 0) {
                            dotProduct += t_features[idx] * t_weights[idx];
                        }
                    }
                }
            }
        }
    }

    // Piece square
    if (FeatureUtils::doesFeatureSetContainFeatureType(featureSet, FeatureType::PIECE_SQUARE)) {
        // Find offset first
        const unsigned int offset = FeatureUtils::getOffsetForFeatureTypeGivenFeatureSet(FeatureType::PIECE_SQUARE, featureSet);
        BoardIndex bi;
        for (bi = 1; bi <= FeatureUtils::NUM_SQUARES; ++bi) {
            const Piece* pieceHere = t_board.getPieceAtPosition(positionIndexesRelativeToPlayer[t_player - 1][bi - 1]);
            if (pieceHere != nullptr) {
                // Offset + RelativePlayer->PieceType->Square
                const unsigned int idx = offset +
                                         (Brain::getSeatsBetweenPlayers(t_player, pieceHere->getOwner()) * FeatureUtils::NUM_UNIQUE_PIECES * FeatureUtils::NUM_SQUARES) +
                                        (FeatureUtils::pieceTypeToIndex(pieceHere->getType()) * FeatureUtils::NUM_SQUARES) +
                                         bi - 1;
                dotProduct += t_features[idx] * t_weights[idx];
            }
        }
    }


    return dotProduct;
}

double Brain::sigmoid(const double x) const {
    return 1.0 / (1.0 + std::exp(-x));
}

void Brain::loadWeights() {
    if (this->getPathToWeights() != "") {
        std::vector<double> weights{};
        std::ifstream infile(this->getPathToWeights());
        std::copy(std::istream_iterator<double>(infile), std::istream_iterator<double>(),
                  std::back_inserter(weights));

        infile.close();
        assert(weights.size() > 0);
        this->setWeights(weights);
        m_weightDelta.reserve(weights.size());
    }
}

// Used for loading a subset of weights and initialising the rest to 0
// Not particularly efficient, but it's not time critical
void Brain::loadWeightsAndFillVoid() {
    const FeatureSet featureSet = this->getFeatureSet();
    unsigned int expectedSize = FeatureUtils::getNumFeaturesAssociatedWithFeatureSet(featureSet);
    this->loadWeights();
    while (m_weights.size() < expectedSize) {
        m_weights.push_back(0);
        m_weightDelta.push_back(0);
    }

}

void Brain::resetWeightDelta() {
    m_weightDelta.assign(this->getWeights().size(), 0);
}

void Brain::applyWeightDelta() {
    const unsigned int numWeights = this->getWeights().size();

    unsigned int i;
    for (i = 0; i < numWeights; ++i) {
        this->getWeights()[i] += this->getWeightDelta()[i];
    }

    this->resetWeightDelta();
}


void Brain::saveWeights() const {
    // If we've specified a separate path to save the new weights, use that
    // Otherwise, overwrite the old ones
    std::string path;
    if (this->getWeightsSavePath() != "") {
        path = this->getWeightsSavePath();
    } else {
        path = this->getPathToWeights();
    }
    const std::vector<double>& weights = this->getWeights();
    std::ofstream outFile(path);
    for (auto const& weight : weights) {
        outFile << weight << " ";
    }
    outFile.close();
}

std::string Brain::weightsToString() const {
    std::stringstream result;

    result << "{ ";
    for (auto const& weight : this->getWeights()) {
        result << weight << " ";
    }
    result << "}";

    return result.str();
}

void Brain::updateWeightsForFeatures(const std::vector<Feature>& t_features, const double t_lowerBound, const double t_upperBound, const int t_depthInTree, const short t_player, const Board& t_board) {
    const double discountRate = this->getDiscountFactorAtDepth(t_depthInTree);
    double targetVal = 0;
    const double heuristicHere = this->multiplyFeaturesByWeights(t_features, this->getWeights(), t_player, t_board);
    if (heuristicHere < t_lowerBound) {
        targetVal = this->sigmoid(t_lowerBound) - this->sigmoid(heuristicHere);
    } else if (heuristicHere > t_upperBound) {
        targetVal = -this->sigmoid(heuristicHere) + this->sigmoid(t_upperBound);
    } else {
        return;
    }

    targetVal *= discountRate;

    // For all active features here, train them towards the target value
    this->addTargetValToWeightDelta(t_features, targetVal, t_player, t_board);

}

// A more efficient way to add the target value to the weights delta
// Rather than iterating through all features, intelligently only visit the active ones
void Brain::addTargetValToWeightDelta(const std::vector<Feature>& t_features, const double t_targetVal, const short t_player, const Board& t_board) {
    const FeatureSet featureSet = this->getFeatureSet();
    const double amountToAdd = t_targetVal * Brain::LEARNING_RATE;
    const double minDelta = -Brain::MAX_WEIGHT_DELTA;
    const double maxDelta = Brain::MAX_WEIGHT_DELTA;

    // Handle material weights
    unsigned int i;
    for (i = FeatureUtils::INDEX_MATERIAL_WEIGHTS_START; i < FeatureUtils::INDEX_ATTACKING_WEIGHTS_START; ++i) {
        if (t_features[i] != 0) {
            m_weightDelta[i] += amountToAdd;
            m_weightDelta[i] = std::min(maxDelta, m_weightDelta[i]);
            m_weightDelta[i] = std::max(minDelta, m_weightDelta[i]);
        }
    }

    if (FeatureUtils::doesFeatureSetContainFeatureType(featureSet, FeatureType::ATTACKING_DEFENDING_NAIVE)) {
        // We're using naive att def weights, so it's more efficient just to iterate along them too
        for (i = FeatureUtils::INDEX_ATTACKING_NAIVE_WEIGHTS_START; i < FeatureUtils::INDEX_PIECE_SQUARE_AFTER_NAIVE_WEIGHTS_START; ++i) {
            if (t_features[i] != 0) {
                m_weightDelta[i] += amountToAdd;
                m_weightDelta[i] = std::min(maxDelta, m_weightDelta[i]);
                m_weightDelta[i] = std::max(minDelta, m_weightDelta[i]);
            }
        }
    } else if (FeatureUtils::doesFeatureSetContainFeatureType(featureSet, FeatureType::ATTACKING_DEFENDING)) {
        // Add for all active att/def features
        short player;
        for (player = 1; player <= FeatureUtils::NUM_PLAYERS; ++player) {
            for (auto const& piece : t_board.getPiecesForPlayer(player)) {
                for (auto const& mv : piece->getActiveAttackingVectors()) {
                    const unsigned int idx = Brain::moveVectorToAttIndex(mv, t_player);
                    m_weightDelta[idx] += amountToAdd;
                    m_weightDelta[idx] = std::min(maxDelta, m_weightDelta[idx]);
                    m_weightDelta[idx] = std::max(minDelta, m_weightDelta[idx]);
                }
                for (auto const& mv : piece->getActiveDefendingVectors()) {
                    const unsigned int idx = Brain::moveVectorToDefIndex(mv, t_player);
                    m_weightDelta[idx] += amountToAdd;
                    m_weightDelta[idx] = std::min(maxDelta, m_weightDelta[idx]);
                    m_weightDelta[idx] = std::max(minDelta, m_weightDelta[idx]);
                }
            }
        }
    }

    // Piece square
    if (FeatureUtils::doesFeatureSetContainFeatureType(featureSet, FeatureType::PIECE_SQUARE)) {
        // Find offset first
        const unsigned int offset = FeatureUtils::getOffsetForFeatureTypeGivenFeatureSet(FeatureType::PIECE_SQUARE, featureSet);
        BoardIndex bi;
        for (bi = 1; bi <= FeatureUtils::NUM_SQUARES; ++bi) {
            const Piece* pieceHere = t_board.getPieceAtPosition(positionIndexesRelativeToPlayer[t_player - 1][bi - 1]);
            if (pieceHere != nullptr) {
                // Offset + RelativePlayer->PieceType->Square
                const unsigned int idx = offset +
                                         (Brain::getSeatsBetweenPlayers(t_player, pieceHere->getOwner()) * FeatureUtils::NUM_UNIQUE_PIECES * FeatureUtils::NUM_SQUARES) +
                                         (FeatureUtils::pieceTypeToIndex(pieceHere->getType()) * FeatureUtils::NUM_SQUARES) +
                                         bi - 1;
                m_weightDelta[idx] += amountToAdd;
                m_weightDelta[idx] = std::min(maxDelta, m_weightDelta[idx]);
                m_weightDelta[idx] = std::max(minDelta, m_weightDelta[idx]);
            }
        }
    }

    // King dist
    if (FeatureUtils::doesFeatureSetContainFeatureType(featureSet, FeatureType::KING_DISTANCE)) {
        short playerOffset;
        for (playerOffset = 0; playerOffset < FeatureUtils::NUM_PLAYERS; ++playerOffset) {
            const short player = Brain::getPlayerOffsetBySeats(t_player, playerOffset);
            // Get enemy kings relative to this player
            std::vector<Piece*> enemyKings;
            short offset;
            for (offset = 1; offset <= 5; offset += 2) {
                enemyKings.push_back(t_board.getPiecesForPlayer(Brain::getPlayerOffsetBySeats(player, offset))[12]);
            }
            // For each of this player's pieces, inc the feature index associated with that piece being DISTANCE from the enemy king
            for (auto const& piece : t_board.getPiecesForPlayer(player)) {
                if (!piece->getPosition()->isOffBoard()) {
                    for (auto const& enemyKing : enemyKings) {
                        const int idx = Brain::getKingDistFeatureIndex(piece->getType(), t_player, player, enemyKing->getOwner(), piece->getPosition(), enemyKing->getPosition());
                        if (idx >= 0) {
                            m_weightDelta[idx] += amountToAdd;
                            m_weightDelta[idx] = std::min(maxDelta, m_weightDelta[idx]);
                            m_weightDelta[idx] = std::max(minDelta, m_weightDelta[idx]);
                        }
                    }
                }
            }
        }
    }

}

// For each depth from 0 to 14 inclusive, this is the DISCOUNT_FACTOR raised to that depth
const double Brain::discountRateAtDepth[15] = {1.0, 0.33333, 0.111111, 0.037037, 0.0123456, 0.00411523, 0.00137174,
                                               0.00045725, 0.00015242, 0.00005081, 0.00001694, 5.65e-6, 1.88e-6, 6.3e-7, 2.1e-7};

// A pre-computed table of the discount factor raised to the power of the given depth
double Brain::getDiscountFactorAtDepth(const int t_depth) const {
    return discountRateAtDepth[std::min(t_depth, 14)];
}


std::vector<Feature> Brain::boardToFeatures(const Board& t_board, const short t_player) const {
    // Set up an array
    std::vector<Feature> features;
    const FeatureSet featureSet = this->getFeatureSet();
    int numFeatures = FeatureUtils::getNumFeaturesAssociatedWithFeatureSet(featureSet);
    assert(numFeatures != 0);
    features.reserve(numFeatures);

    // Fill it with numFeatures number of 0s
    features.assign(numFeatures, 0);

    this->fillMaterialFeatures(features, t_board, t_player);
    this->fillPartnerWizardFeatures(features, t_board, t_player);
    if (FeatureUtils::doesFeatureSetContainFeatureType(featureSet, FeatureType::PAWN_DISTANCE)) {
        // Pawn distance features are a subset of the functionality of piece-square weights
        // So don't fill them if we're going piece-square
        this->fillPawnDistanceFeatures(features, t_board, t_player);
    }
    this->fillBiasFeatures(features);
    this->fillCheckFeatures(features, t_board, t_player);
    this->fillCheckmateFeatures(features, t_board, t_player);

    if (FeatureUtils::doesFeatureSetContainFeatureType(featureSet, FeatureType::ATTACKING_DEFENDING)) {
        this->fillAttackingAndDefendingFeatures(features, t_board, t_player);
    } else if (FeatureUtils::doesFeatureSetContainFeatureType(featureSet, FeatureType::ATTACKING_DEFENDING_NAIVE)) {
        this->fillAttackingAndDefendingFeaturesNaive(features, t_board, t_player);
    }

    if (FeatureUtils::doesFeatureSetContainFeatureType(featureSet, FeatureType::KING_DISTANCE)) {
        this->fillKingDistFeatures(features, t_board, t_player);
    }

    if (FeatureUtils::doesFeatureSetContainFeatureType(featureSet, FeatureType::PIECE_SQUARE)) {
        this->fillPieceSquareFeatures(features, t_board, t_player);
    }

    // Evaluate the board

    return features;

}

void Brain::fillMaterialFeatures(std::vector<Feature>& t_features, const Board& t_board, const short t_player) const {
    // For each non-king piece in the game (not offboard), add 1 if it's on my team, subtract 1 if it's not
    const int indexOffset = FeatureUtils::INDEX_MATERIAL_WEIGHTS_START;
    short player;
    for (player = 1; player <= FeatureUtils::NUM_PLAYERS; ++player) {
        bool sameTeam = TeamUtils::isSameTeam(player, t_player);
        for (auto const& piece : t_board.getPiecesForPlayer(player)) {
            const PieceType pieceType = piece->getType();
            if (pieceType != PieceType::KING && !piece->getPosition()->isOffBoard()) {
                const int index = FeatureUtils::pieceTypeToIndex(pieceType) + indexOffset;
                const int modifier = sameTeam ? 1 : -1;
                t_features[index] += modifier;
            }
        }
    }

}

void Brain::fillPartnerWizardFeatures(std::vector<Feature>& t_features, const Board& t_board, const short t_player) const {
    Feature numWizardsOnMyTeam = 0;
    Feature numWizardsOnEnemyTeam = 0;
    short player;
    for (player = 1; player <= FeatureUtils::NUM_PLAYERS; ++player) {
        const bool hasWizard = !t_board.getPiecesForPlayer(player)[11]->getPosition()->isOffBoard();
        if (hasWizard) {
            if (TeamUtils::isSameTeam(player, t_player)) {
                ++numWizardsOnMyTeam;
            } else {
                ++numWizardsOnEnemyTeam;
            }
        }
    }

    // If I have two wizards, I want the value to be 1, if I have all 3, val should be 2, otherwise, 0
    numWizardsOnMyTeam = numWizardsOnMyTeam > 0 ? numWizardsOnMyTeam - 1 : numWizardsOnMyTeam;
    numWizardsOnEnemyTeam = numWizardsOnEnemyTeam > 0 ? numWizardsOnEnemyTeam - 1 : numWizardsOnEnemyTeam;

    t_features[FeatureUtils::INDEX_PARTNER_WIZARD_WEIGHTS_START] = numWizardsOnMyTeam;
    t_features[FeatureUtils::INDEX_PARTNER_WIZARD_WEIGHTS_START + 1] = numWizardsOnEnemyTeam;

}

void Brain::fillPawnDistanceFeatures(std::vector<Feature>& t_features, const Board& t_board, const short t_player) const {
    // For all pawns on board, add 9 minus their distance to the vortex (subtract for pawns on other team)
    short player;
    for (player = 1; player <= FeatureUtils::NUM_PLAYERS; ++player) {
        const bool sameTeam = TeamUtils::isSameTeam(player, t_player);
        int pawnIndex;
        for (pawnIndex = 0; pawnIndex < 5; ++pawnIndex) {
            const Piece* pawn = t_board.getPiecesForPlayer(player)[pawnIndex];
            const Position* pos = pawn->getPosition();
            if (!pos->isOffBoard()) {
                const int stepsToVortex = PositionUtils::distanceToVortex(pos);
                const int val = 4 - stepsToVortex;

                t_features[FeatureUtils::INDEX_PAWN_DISTANCE_WEIGHTS_START] += sameTeam ? val : -val;
            }
        }
    }
}

void Brain::fillBiasFeatures(std::vector<Feature>& t_features) const {
    // The bias feature is always evaluated as a 1
    t_features[FeatureUtils::INDEX_BIAS_WEIGHTS_START] = 1;
}

void Brain::fillCheckFeatures(std::vector<Feature>& t_features, const Board& t_board, const short t_player) const {
    short tempPlayer = t_player - 1;
    short playersChecked;
    int index = FeatureUtils::INDEX_CHECK_WEIGHTS_START;
    for (playersChecked = 0; playersChecked < FeatureUtils::NUM_PLAYERS; ++playersChecked, ++index) {
        tempPlayer = t_player + playersChecked;
        tempPlayer = tempPlayer > 6 ? tempPlayer - 6 : tempPlayer;
        if (t_board.isPlayerInCheck(tempPlayer)) {
            t_features[index] = 1;
        }
    }
}

void Brain::fillCheckmateFeatures(std::vector<Feature>& t_features, const Board& t_board, const short t_player) const {
    short tempPlayer = t_player - 1;
    short playersChecked = 0;
    int index = FeatureUtils::INDEX_CHECKMATE_WEIGHTS_START;
    for (playersChecked = 0; playersChecked < FeatureUtils::NUM_PLAYERS; ++playersChecked, ++index) {
        tempPlayer = t_player + playersChecked;
        tempPlayer = tempPlayer > 6 ? tempPlayer - 6 : tempPlayer;
        if (t_board.isPlayerInCheckmate(tempPlayer)) {
            t_features[index] = 1;
        }
    }
}

void Brain::fillAttackingAndDefendingFeaturesNaive(std::vector<Feature>& t_features, const Board& t_board, const short t_player) const {
    short player;
    for (player = 1; player <= FeatureUtils::NUM_PLAYERS; ++player) {
        for (auto const& piece : t_board.getPiecesForPlayer(player)) {
            for (auto const& mv : piece->getActiveAttackingVectors()) {
                t_features[Brain::moveVectorToNaiveAttIndex(mv, t_player)]++;
            }
            for (auto const& mv : piece->getActiveDefendingVectors()) {
                t_features[Brain::moveVectorToNaiveDefIndex(mv, t_player)]++;
            }
        }
    }
}

void Brain::fillAttackingAndDefendingFeatures(std::vector<Feature>& t_features, const Board& t_board, const short t_player) const {
    short player;
    for (player = 1; player <= FeatureUtils::NUM_PLAYERS; ++player) {
        for (auto const& piece : t_board.getPiecesForPlayer(player)) {
            for (auto const& mv : piece->getActiveAttackingVectors()) {
                t_features[Brain::moveVectorToAttIndex(mv, t_player)]++;
            }
            for (auto const& mv : piece->getActiveDefendingVectors()) {
                t_features[Brain::moveVectorToDefIndex(mv, t_player)]++;
            }
        }
    }
}

void Brain::fillKingDistFeatures(std::vector<Feature>& t_features, const Board& t_board, const short t_player) const {
    short playerOffset;
    for (playerOffset = 0; playerOffset < FeatureUtils::NUM_PLAYERS; ++playerOffset) {
        const short player = Brain::getPlayerOffsetBySeats(t_player, playerOffset);
        // Get enemy kings relative to this player
        std::vector<Piece*> enemyKings;
        short offset;
        for (offset = 1; offset <= 5; offset += 2) {
            enemyKings.push_back(t_board.getPiecesForPlayer(Brain::getPlayerOffsetBySeats(player, offset))[12]);
        }
        // For each of this player's pieces, inc the feature index associated with that piece being DISTANCE from the enemy king
        for (auto const& piece : t_board.getPiecesForPlayer(player)) {
            if (!piece->getPosition()->isOffBoard()) {
                for (auto const& enemyKing : enemyKings) {
                    const int idx = Brain::getKingDistFeatureIndex(piece->getType(), t_player, player, enemyKing->getOwner(), piece->getPosition(), enemyKing->getPosition());
                    if (idx >= 0) {
                        t_features[idx]++;
                    }
                }
            }
        }
    }
}

void Brain::fillPieceSquareFeatures(std::vector<Feature>& t_features, const Board& t_board, const short t_player) const {
    const int baseOffset = FeatureUtils::getOffsetForFeatureTypeGivenFeatureSet(FeatureType::PIECE_SQUARE, this->getFeatureSet());
    BoardIndex bi;
    for (bi = 1; bi <= FeatureUtils::NUM_SQUARES; ++bi) {
        const Piece* pieceHere = t_board.getPieceAtPosition(positionIndexesRelativeToPlayer[t_player - 1][bi - 1]);
        if (pieceHere != nullptr) {
            // Offset + RelativePlayer->PieceType->Square
            t_features[baseOffset +
                       (Brain::getSeatsBetweenPlayers(t_player, pieceHere->getOwner()) * FeatureUtils::NUM_UNIQUE_PIECES * FeatureUtils::NUM_SQUARES) +
                        (FeatureUtils::pieceTypeToIndex(pieceHere->getType()) * FeatureUtils::NUM_SQUARES) +
                       bi - 1] = 1;
        }
    }

}

int Brain::getRandomNumber(const int limit) const {
    return std::rand() % limit;
}


void Brain::addLineToSearchLogs(const std::string t_logLine) {
    m_searchAnalyticsLogs.push_back(t_logLine);
}

void Brain::writeLogs() {

    const std::string dest = this->getLogDestination();

    if (dest != "") {
        std::ofstream outFile(dest);
        // Write CSV Header
        outFile << SearchAnalytics::getHeader() << std::endl;

        // Write values
        for (auto const logLine : m_searchAnalyticsLogs) {
            outFile << logLine << std::endl;
        }
        outFile.close();
    }
}

// Given a base player, tells you the player "t_seatsAround" seats away from them
short Brain::getPlayerOffsetBySeats(const short t_basePlayer, const short t_seatsAround) {
    assert(t_seatsAround < 6 && t_seatsAround >= 0);
    const short relativePlayer = t_basePlayer + t_seatsAround;
    return relativePlayer > 6 ? relativePlayer - 6 : relativePlayer;
}

short Brain::getSeatsBetweenPlayers(const short t_playerA, const short t_playerB) {
    const short distance = t_playerB - t_playerA;
    return distance < 0 ? distance + 6 : distance;
}

const BoardIndex Brain::positionIndexesRelativeToPlayer[6][6 * 157] = {
       {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157},
       {21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 121, 122, 123, 124, 125, 126, 157},
       {41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 157},
       {61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 157},
       {81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 157},
       {101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 151, 152, 153, 154, 155, 156, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 157}
};

unsigned int Brain::moveVectorToAttIndex(const MoveVector* t_moveVector, const short t_player) {
    const Piece* activePiece = t_moveVector->getActivePiece();
    const Piece* passivePiece = t_moveVector->getPassivePiece();
    return FeatureUtils::INDEX_ATTACKING_WEIGHTS_START +
            Brain::getSeatsBetweenPlayers(t_player, activePiece->getOwner()) * FeatureUtils::NUM_UNIQUE_PIECES * (FeatureUtils::NUM_PLAYERS / 2) * FeatureUtils::NUM_UNIQUE_PIECES +
           FeatureUtils::pieceTypeToIndex(activePiece->getType()) * (FeatureUtils::NUM_PLAYERS / 2) * FeatureUtils::NUM_UNIQUE_PIECES +
            ((Brain::getSeatsBetweenPlayers(activePiece->getOwner(), passivePiece->getOwner()) - 1) / 2) * FeatureUtils::NUM_UNIQUE_PIECES +
            FeatureUtils::pieceTypeToIndex(passivePiece->getType());
}

unsigned int Brain::moveVectorToNaiveAttIndex(const MoveVector* t_moveVector, const short t_player) {
    const Piece* activePiece = t_moveVector->getActivePiece();
    const Piece* passivePiece = t_moveVector->getPassivePiece();
    const unsigned int teamMultiplier = TeamUtils::isSameTeam(activePiece->getOwner(), t_player) ? 0 : 1;
    return FeatureUtils::INDEX_ATTACKING_NAIVE_WEIGHTS_START +
           teamMultiplier * FeatureUtils::NUM_UNIQUE_PIECES * FeatureUtils::NUM_UNIQUE_PIECES +
           FeatureUtils::pieceTypeToIndex(activePiece->getType()) * FeatureUtils::NUM_UNIQUE_PIECES +
           FeatureUtils::pieceTypeToIndex(passivePiece->getType());
}

unsigned int Brain::moveVectorToDefIndex(const MoveVector* t_moveVector, const short t_player) {
    const Piece* activePiece = t_moveVector->getActivePiece();
    const Piece* passivePiece = t_moveVector->getPassivePiece();
    return FeatureUtils::INDEX_DEFENDING_WEIGHTS_START +
            (Brain::getSeatsBetweenPlayers(t_player, activePiece->getOwner()) * FeatureUtils::NUM_UNIQUE_PIECES * (FeatureUtils::NUM_PLAYERS / 2) * FeatureUtils::NUM_UNIQUE_PIECES) +
            (FeatureUtils::pieceTypeToIndex(activePiece->getType()) * (FeatureUtils::NUM_PLAYERS / 2) * FeatureUtils::NUM_UNIQUE_PIECES) +
           (((Brain::getSeatsBetweenPlayers(activePiece->getOwner(), passivePiece->getOwner())) / 2) * FeatureUtils::NUM_UNIQUE_PIECES) +
           FeatureUtils::pieceTypeToIndex(passivePiece->getType());
}

unsigned int Brain::moveVectorToNaiveDefIndex(const MoveVector* t_moveVector, const short t_player) {
    const Piece* activePiece = t_moveVector->getActivePiece();
    const Piece* passivePiece = t_moveVector->getPassivePiece();
    const unsigned int teamMultiplier = TeamUtils::isSameTeam(activePiece->getOwner(), t_player) ? 0 : 1;
    return FeatureUtils::INDEX_DEFENDING_NAIVE_WEIGHTS_START +
           teamMultiplier * FeatureUtils::NUM_UNIQUE_PIECES * FeatureUtils::NUM_UNIQUE_PIECES +
           FeatureUtils::pieceTypeToIndex(activePiece->getType()) * FeatureUtils::NUM_UNIQUE_PIECES +
           FeatureUtils::pieceTypeToIndex(passivePiece->getType());
}

int Brain::getKingDistFeatureIndex(const PieceType t_pieceType, const short t_basePlayer, const short t_pieceOwner, const short t_kingOwner, const Position* t_piecePosition, const Position* t_kingPosition) {
    assert(t_piecePosition->getBoardIndex() != t_kingPosition->getBoardIndex());
    assert(!t_piecePosition->isOffBoard());
    assert(!TeamUtils::isSameTeam(t_pieceOwner, t_kingOwner));

    const short pieceIndex = t_pieceType == PieceType::KING ? FeatureUtils::WIZARD_INDEX : FeatureUtils::pieceTypeToIndex(t_pieceType);

    const short steps = PositionUtils::stepsToPosition(pieceIndex, t_piecePosition, t_kingPosition);

    // If the piece can't access the king, return -1
    if (steps <= 0) {
        return -1;
    }

    unsigned int pieceOffset = 0;
    if (t_pieceType != PieceType::PAWN) {
        pieceOffset += FeatureUtils::MAX_DIST_PAWN;
        if (t_pieceType != PieceType::BISHOP) {
            pieceOffset += FeatureUtils::MAX_DIST_BISHOP;
            if (t_pieceType != PieceType::KNIGHT) {
                pieceOffset += FeatureUtils::MAX_DIST_KNIGHT;
                if (t_pieceType != PieceType::QUEEN) {
                    pieceOffset += FeatureUtils::MAX_DIST_QUEEN;
                    if (t_pieceType != PieceType::ROOK) {
                        pieceOffset += FeatureUtils::MAX_DIST_ROOK;
                        if (t_pieceType != PieceType::DUCHESS) {
                            pieceOffset += FeatureUtils::MAX_DIST_DUCHESS;
                            if (t_pieceType != PieceType::WIZARD && t_pieceType != PieceType::KING) {
                                pieceOffset += FeatureUtils::MAX_DIST_WIZARD;
                            }
                        }
                    }
                }
            }
        }
    }

    // Player->Opponent->PieceType->(NumSteps - 1)
    return FeatureUtils::INDEX_KING_DIST_WEIGHTS_START +
           (
               (Brain::getSeatsBetweenPlayers(t_basePlayer, t_pieceOwner) * FeatureUtils::NUM_PLAYERS / 2 * FeatureUtils::NUM_KING_DIST_ENTRIES_PER_PIECE) +
               ((Brain::getSeatsBetweenPlayers(t_pieceOwner, t_kingOwner) / 2) * FeatureUtils::NUM_KING_DIST_ENTRIES_PER_PIECE) +
               pieceOffset +
               steps - 1 // Subtract 1 since the 0 case is redundant
           );
}