#define NDEBUG

#include "FeatureUtils.h"
#include "../duchess/PieceType.h"

#include <vector>
#include <cassert>

int FeatureUtils::pieceTypeToIndex(const PieceType& t_pieceType) {
    if (t_pieceType == PieceType::PAWN) {
        return FeatureUtils::PAWN_INDEX;
    } else if (t_pieceType == PieceType::KING) {
        return FeatureUtils::KING_INDEX;
    } else if (t_pieceType == PieceType::QUEEN) {
        return FeatureUtils::QUEEN_INDEX;
    } else if (t_pieceType == PieceType::DUCHESS) {
        return FeatureUtils::DUCHESS_INDEX;
    } else if (t_pieceType == PieceType::FORTRESS) {
        return FeatureUtils::FORTRESS_INDEX;
    } else if (t_pieceType == PieceType::WIZARD) {
        return FeatureUtils::WIZARD_INDEX;
    } else if (t_pieceType == PieceType::BISHOP) {
        return FeatureUtils::BISHOP_INDEX;
    } else if (t_pieceType == PieceType::ROOK) {
        return FeatureUtils::ROOK_INDEX;
    } else { // KNIGHT
        return FeatureUtils::KNIGHT_INDEX;
    }
}

bool FeatureUtils::doesFeatureSetContainFeatureType(const FeatureSet t_featureSet, const FeatureType t_featureType) {
    switch (t_featureSet) {
        case FeatureSet::MATERIAL:
            return t_featureType == FeatureType::MATERIAL ||
                   t_featureType == FeatureType::PARTNER_WIZARD ||
                   t_featureType == FeatureType::PAWN_DISTANCE ||
                   t_featureType == FeatureType::BIAS ||
                   t_featureType == FeatureType::CHECK ||
                   t_featureType == FeatureType::CHECKMATE;
        case FeatureSet::MATERIAL_ATTDEF:
            return t_featureType == FeatureType::ATTACKING_DEFENDING ||
                    FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL, t_featureType);
        case FeatureSet::MATERIAL_NAIVEATTDEF:
            return t_featureType == FeatureType::ATTACKING_DEFENDING_NAIVE ||
                   FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL, t_featureType);
        case FeatureSet::MATERIAL_ATTDEF_PIECESQUARE:
            return t_featureType == FeatureType::PIECE_SQUARE ||
                    (t_featureType != FeatureType::PAWN_DISTANCE && FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL_ATTDEF, t_featureType));
        case FeatureSet::MATERIAL_NAIVEATTDEF_PIECESQUARE:
            return t_featureType == FeatureType::PIECE_SQUARE ||
                    (t_featureType != FeatureType::PAWN_DISTANCE && FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL_NAIVEATTDEF, t_featureType));
        case FeatureSet::MATERIAL_ATTDEF_KINGDIST:
            return t_featureType == FeatureType::KING_DISTANCE ||
                   FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL_ATTDEF, t_featureType);
        case FeatureSet::MATERIAL_ATTDEF_KINGDIST_PIECE_SQUARE:
            return t_featureType == FeatureType::PIECE_SQUARE ||
                    (t_featureType != FeatureType::PAWN_DISTANCE && FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL_ATTDEF_KINGDIST, t_featureType));
        case FeatureSet::ALL:
            return true;
        default:
            return false;
    }
}

unsigned int FeatureUtils::getNumFeaturesAssociatedWithFeatureSet(const FeatureSet t_featureSet) {
    if (t_featureSet == FeatureSet::MATERIAL) {
        return FeatureUtils::INDEX_ATTACKING_WEIGHTS_START;
    } else if (t_featureSet == FeatureSet::MATERIAL_ATTDEF) {
        return FeatureUtils::INDEX_PIECE_SQUARE_WEIGHTS_START;
    } else if (t_featureSet == FeatureSet::MATERIAL_ATTDEF_PIECESQUARE || t_featureSet == FeatureSet::ALL) {
        return FeatureUtils::TOTAL_WEIGHTS;
    } else if (t_featureSet == FeatureSet::MATERIAL_NAIVEATTDEF) {
        return FeatureUtils::INDEX_PIECE_SQUARE_AFTER_NAIVE_WEIGHTS_START;
    } else if (t_featureSet == FeatureSet::MATERIAL_NAIVEATTDEF_PIECESQUARE) {
        return FeatureUtils::TOTAL_WEIGHTS_NAIVE;
    } else if (t_featureSet == FeatureSet::MATERIAL_ATTDEF_KINGDIST) {
        return FeatureUtils::INDEX_PIECE_SQUARE_AFTER_KING_DIST_START;
    } else if (t_featureSet == FeatureSet::MATERIAL_ATTDEF_KINGDIST_PIECE_SQUARE) {
        return FeatureUtils::TOTAL_WEIGHTS_KING_DIST;
    } else {
        assert(false);
        return 0;
    }
}

unsigned int FeatureUtils::getOffsetForFeatureTypeGivenFeatureSet(const FeatureType t_featureType, const FeatureSet t_featureSet) {
    switch(t_featureType) {
        case FeatureType::PIECE_SQUARE:
            switch (t_featureSet) {
                case FeatureSet::MATERIAL_ATTDEF_KINGDIST_PIECE_SQUARE:
                    return FeatureUtils::INDEX_PIECE_SQUARE_AFTER_KING_DIST_START;
                case FeatureSet::MATERIAL_ATTDEF_PIECESQUARE:
                    return FeatureUtils::INDEX_PIECE_SQUARE_WEIGHTS_START;
                case FeatureSet::MATERIAL_NAIVEATTDEF_PIECESQUARE:
                    return FeatureUtils::INDEX_PIECE_SQUARE_AFTER_NAIVE_WEIGHTS_START;
                default:
                    assert(false);
                    return 0;
            }
        case FeatureType::MATERIAL:
            return FeatureUtils::INDEX_MATERIAL_WEIGHTS_START;
        case FeatureType::CHECK:
            return FeatureUtils::INDEX_CHECK_WEIGHTS_START;
        case FeatureType::CHECKMATE:
            return FeatureUtils::INDEX_CHECKMATE_WEIGHTS_START;
        case FeatureType::BIAS:
            return FeatureUtils::INDEX_BIAS_WEIGHTS_START;
        case FeatureType::PARTNER_WIZARD:
            return FeatureUtils::INDEX_PARTNER_WIZARD_WEIGHTS_START;
        case FeatureType::PAWN_DISTANCE:
            return FeatureUtils::INDEX_PAWN_DISTANCE_WEIGHTS_START;
        case FeatureType::ATTACKING_DEFENDING:
            return FeatureUtils::INDEX_ATTACKING_WEIGHTS_START;
        case FeatureType::ATTACKING_DEFENDING_NAIVE:
            return FeatureUtils::INDEX_ATTACKING_NAIVE_WEIGHTS_START;
        case FeatureType::KING_DISTANCE:
            return FeatureUtils::INDEX_KING_DIST_WEIGHTS_START;
        default:
            assert(false);
            return 0;
    }
}