#ifndef DUCHESS_CPP_FEATUREUTILS_H
#define DUCHESS_CPP_FEATUREUTILS_H

#include "../duchess/PieceType.h"
#include "../duchess/Position.h"
#include "FeatureType.h"
#include "FeatureSet.h"

class FeatureUtils {
public:
    static int pieceTypeToIndex(const PieceType& t_pieceType);
    static bool doesFeatureSetContainFeatureType(const FeatureSet t_featureSet, const FeatureType t_featureType);
    static unsigned int getNumFeaturesAssociatedWithFeatureSet(const FeatureSet t_featureSet);
    static unsigned int getOffsetForFeatureTypeGivenFeatureSet(const FeatureType t_featureType, const FeatureSet t_featureSet);

    static const int PIECES_PER_PLAYER {15};
    static const int NUM_UNIQUE_PIECES {9};
    static const int NUM_TEAMS {2};
    static const int NUM_PLAYERS {6};
    static const int NUM_SQUARES {157};
    static const int NUM_PIECES {PIECES_PER_PLAYER * NUM_PLAYERS};

    static const int NUM_MATERIAL_WEIGHTS {NUM_UNIQUE_PIECES - 1}; // No weight for kings
    static const int NUM_PAWN_DISTANCE_WEIGHTS {1};
    static const int NUM_BIAS_WEIGHTS {1};

    static const int NUM_ATTACKING_WEIGHTS {(NUM_PLAYERS) * NUM_UNIQUE_PIECES * (NUM_PLAYERS / 2) * NUM_UNIQUE_PIECES}; // Player->PieceType->EnemyPlayer->PieceType
    static const int NUM_NAIVE_ATTACKING_WEIGHTS {NUM_TEAMS * NUM_UNIQUE_PIECES * NUM_UNIQUE_PIECES}; // Team->PieceType->PieceType
    static const int NUM_DEFENDING_WEIGHTS {NUM_ATTACKING_WEIGHTS};
    static const int NUM_NAIVE_DEFENDING_WEIGHTS {NUM_NAIVE_ATTACKING_WEIGHTS};

    static const int MAX_DIST_PAWN {17};
    static const int MAX_DIST_BISHOP {6};
    static const int MAX_DIST_KNIGHT {8};
    static const int MAX_DIST_QUEEN {4};
    static const int MAX_DIST_ROOK {4};
    static const int MAX_DIST_DUCHESS {6};
    static const int MAX_DIST_WIZARD {13};
    static const int MAX_DIST_FORTRESS {4};
    static const int NUM_KING_DIST_ENTRIES_PER_PIECE {MAX_DIST_PAWN +
                                                      MAX_DIST_BISHOP +
                                                      MAX_DIST_KNIGHT +
                                                      MAX_DIST_QUEEN +
                                                      MAX_DIST_ROOK +
                                                      MAX_DIST_DUCHESS +
                                                      MAX_DIST_WIZARD +
                                                      MAX_DIST_FORTRESS};

    static const int NUM_KING_DIST_WEIGHTS {NUM_PLAYERS * (NUM_PLAYERS / 2) * NUM_KING_DIST_ENTRIES_PER_PIECE}; // Player->Opponent->PieceType->(NumSteps - 1)
    static const int NUM_PIECE_SQUARE_WEIGHTS {NUM_PLAYERS * NUM_UNIQUE_PIECES * NUM_SQUARES}; // Player->PieceType->Square

    // Material
    static const int INDEX_MATERIAL_WEIGHTS_START {0};
    // Partner wizards
    static const int INDEX_PARTNER_WIZARD_WEIGHTS_START {INDEX_MATERIAL_WEIGHTS_START + NUM_MATERIAL_WEIGHTS};
    // Pawn distance to vortex
    static const int INDEX_PAWN_DISTANCE_WEIGHTS_START {INDEX_PARTNER_WIZARD_WEIGHTS_START + NUM_TEAMS};
    // Bias
    static const int INDEX_BIAS_WEIGHTS_START {INDEX_PAWN_DISTANCE_WEIGHTS_START + NUM_PAWN_DISTANCE_WEIGHTS};
    // Check
    static const int INDEX_CHECK_WEIGHTS_START {INDEX_BIAS_WEIGHTS_START + NUM_BIAS_WEIGHTS};
    // Checkmate
    static const int INDEX_CHECKMATE_WEIGHTS_START {INDEX_CHECK_WEIGHTS_START + NUM_PLAYERS};
    // Attacking
    static const int INDEX_ATTACKING_WEIGHTS_START {INDEX_CHECKMATE_WEIGHTS_START + NUM_PLAYERS};
    // Defending
    static const int INDEX_DEFENDING_WEIGHTS_START {INDEX_ATTACKING_WEIGHTS_START + NUM_ATTACKING_WEIGHTS};
    // Attacking naive
    static const int INDEX_ATTACKING_NAIVE_WEIGHTS_START {INDEX_ATTACKING_WEIGHTS_START};
    // Defending naive
    static const int INDEX_DEFENDING_NAIVE_WEIGHTS_START {INDEX_ATTACKING_WEIGHTS_START + NUM_NAIVE_ATTACKING_WEIGHTS};
    // King dist
    static const int INDEX_KING_DIST_WEIGHTS_START {INDEX_DEFENDING_WEIGHTS_START + NUM_DEFENDING_WEIGHTS};
    // Piece-square
    static const int INDEX_PIECE_SQUARE_WEIGHTS_START {INDEX_DEFENDING_WEIGHTS_START + NUM_DEFENDING_WEIGHTS};

    static const int INDEX_PIECE_SQUARE_AFTER_NAIVE_WEIGHTS_START {INDEX_DEFENDING_NAIVE_WEIGHTS_START + NUM_NAIVE_DEFENDING_WEIGHTS};
    static const int INDEX_PIECE_SQUARE_AFTER_KING_DIST_START {INDEX_KING_DIST_WEIGHTS_START + NUM_KING_DIST_WEIGHTS};

    static const int TOTAL_WEIGHTS {INDEX_PIECE_SQUARE_WEIGHTS_START + NUM_PIECE_SQUARE_WEIGHTS};
    static const int TOTAL_WEIGHTS_NAIVE {INDEX_PIECE_SQUARE_AFTER_NAIVE_WEIGHTS_START + NUM_PIECE_SQUARE_WEIGHTS};
    static const int TOTAL_WEIGHTS_KING_DIST {INDEX_PIECE_SQUARE_AFTER_KING_DIST_START + NUM_PIECE_SQUARE_WEIGHTS};

    static const int PAWN_INDEX {0};
    static const int BISHOP_INDEX {1};
    static const int KNIGHT_INDEX {2};
    static const int QUEEN_INDEX {3};
    static const int ROOK_INDEX {4};
    static const int DUCHESS_INDEX {5};
    static const int WIZARD_INDEX {6};
    static const int FORTRESS_INDEX {7};
    static const int KING_INDEX {8};

};

#endif //DUCHESS_CPP_FEATUREUTILS_H
