#include "testFeatureUtils.h"

#include "../FeatureUtils.h"
#include "../FeatureSet.h"
#include "../FeatureType.h"
#include "../../duchess/PieceType.h"

#include <cassert>

void TestFeatureUtils::runTests() {
    TestFeatureUtils::test_pieceTypeToIndex();
    TestFeatureUtils::test_doesFeatureSetContainFeatureType();
    TestFeatureUtils::test_getNumFeaturesAssociatedWithFeatureSet();
}

void TestFeatureUtils::test_pieceTypeToIndex() {
    assert(FeatureUtils::pieceTypeToIndex(PieceType::PAWN) == 0);
    assert(FeatureUtils::pieceTypeToIndex(PieceType::KING) == 8);
}

void TestFeatureUtils::test_doesFeatureSetContainFeatureType() {
    // todo fix
    assert(FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL, FeatureType::BIAS));
    assert(FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL, FeatureType::PARTNER_WIZARD));
    assert(FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL_ATTDEF, FeatureType::PARTNER_WIZARD));
    assert(FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL_NAIVEATTDEF, FeatureType::PARTNER_WIZARD));
    assert(FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::ALL, FeatureType::PARTNER_WIZARD));
    assert(FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL, FeatureType::PAWN_DISTANCE));
    assert(FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL_ATTDEF_KINGDIST, FeatureType::PAWN_DISTANCE));
    assert(FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL_ATTDEF, FeatureType::PAWN_DISTANCE));
    assert(FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL_ATTDEF_KINGDIST, FeatureType::KING_DISTANCE));
    assert(FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL_ATTDEF_KINGDIST, FeatureType::ATTACKING_DEFENDING));
    assert(!FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL_ATTDEF_KINGDIST, FeatureType::ATTACKING_DEFENDING_NAIVE));
    assert(!FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL_ATTDEF, FeatureType::KING_DISTANCE));
    assert(FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL_ATTDEF_KINGDIST_PIECE_SQUARE, FeatureType::PIECE_SQUARE));
    assert(!FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL_ATTDEF_KINGDIST_PIECE_SQUARE, FeatureType::PAWN_DISTANCE));
    assert(!FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL_NAIVEATTDEF_PIECESQUARE, FeatureType::PAWN_DISTANCE));
    assert(!FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL_ATTDEF_PIECESQUARE, FeatureType::PAWN_DISTANCE));
    assert(!FeatureUtils::doesFeatureSetContainFeatureType(FeatureSet::MATERIAL_ATTDEF_KINGDIST, FeatureType::PIECE_SQUARE));
}

void TestFeatureUtils::test_getNumFeaturesAssociatedWithFeatureSet() {
    assert(FeatureUtils::getNumFeaturesAssociatedWithFeatureSet(FeatureSet::MATERIAL) == FeatureUtils::INDEX_ATTACKING_NAIVE_WEIGHTS_START);
    assert(FeatureUtils::getNumFeaturesAssociatedWithFeatureSet(FeatureSet::ALL) == FeatureUtils::TOTAL_WEIGHTS);
    assert(FeatureUtils::getNumFeaturesAssociatedWithFeatureSet(FeatureSet::MATERIAL_ATTDEF_PIECESQUARE) == FeatureUtils::TOTAL_WEIGHTS);
    assert(FeatureUtils::getNumFeaturesAssociatedWithFeatureSet(FeatureSet::MATERIAL_ATTDEF_KINGDIST_PIECE_SQUARE) == FeatureUtils::TOTAL_WEIGHTS_KING_DIST);
    assert(FeatureUtils::getNumFeaturesAssociatedWithFeatureSet(FeatureSet::MATERIAL_ATTDEF_KINGDIST) == FeatureUtils::INDEX_PIECE_SQUARE_AFTER_KING_DIST_START);
}