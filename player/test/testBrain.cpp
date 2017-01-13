#include "testBrain.h"

#include "../../duchess/Board.h"
#include "../../duchess/Piece.h"
#include "../../duchess/Position.h"

#include "../../player/Brain.h"
#include "../../player/Heuristic.h"
#include "../../player/FeatureSet.h"
#include "../../player/FeatureSet.h"
#include "../../player/FeatureUtils.h"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

void TestBrain::runTests() {
    TestBrain::test_settersAndGetters();
    TestBrain::test_sigmoid();
    TestBrain::test_evaluateBoardState();
    TestBrain::test_chanceOfWinningForBoard();
    TestBrain::test_saveLoadWeights();
    TestBrain::test_boardToFeatures();
    TestBrain::test_updateWeightsForFeatures();
    TestBrain::test_randomNumber();
    TestBrain::test_sortMovesBasedOnHeuristic();
    TestBrain::test_getPlayerOffsetBySeats();
    TestBrain::test_seatsBetweenPlayers();
    TestBrain::test_positionIndexesRelativeToPlayerLookup();
    TestBrain::test_naiveAttDef();
    TestBrain::test_attDef();
    TestBrain::test_pieceSquare();
    TestBrain::test_addTargetValToWeightDelta();
    TestBrain::test_kingDist();
}

void TestBrain::test_settersAndGetters() {
    Brain brain;
    assert(brain.getFeatureSet() == FeatureSet::MATERIAL); // MATERIAL Default
    assert(brain.getHeuristic() == Heuristic::NONE); // NONE Default
    assert(brain.getPathToWeights() == "");

    brain.setFeatureSet(FeatureSet::ALL);
    brain.setHeuristic(Heuristic::HISTORY);
    assert(brain.getFeatureSet() == FeatureSet::ALL);
    assert(brain.getHeuristic() == Heuristic::HISTORY);

    std::vector<double> fakeWeights {1.1, 2.2};
    brain.setWeights(fakeWeights);
    assert(brain.getWeights() == fakeWeights);

    std::string pathToWeights1 = "../player/test/resources/testWeights1.txt";
    std::string pathToWeights2 = "../player/test/resources/testWeights2.txt";
    Brain brain2 (pathToWeights1);
    assert(brain2.getPathToWeights() == pathToWeights1);
    brain2.setPathToWeights(pathToWeights2);
    assert(brain2.getPathToWeights() == pathToWeights2);

}

void TestBrain::test_sigmoid() {
    Brain brain;
    double s1 = brain.sigmoid(2);
    double s2 = brain.sigmoid(0);
    double s3 = brain.sigmoid(-1);
    assert(s1 > 0.8807 && s1 < 0.8808);
    assert(s2 == 0.5);
    assert(s3 > 0.2689 && s3 < 0.2690);
}

void TestBrain::test_evaluateBoardState() {
    assert(true);
}

void TestBrain::test_chanceOfWinningForBoard() {
    assert(true);
}

void TestBrain::test_saveLoadWeights() {
    std::string pathToWeights1 = "../player/test/resources/testWeights1.txt";
    // Note that constructor calls loadWeights
    Brain brain (pathToWeights1);

    // Check that the weights match the contents of testWeights1
    std::vector<double> expectedWeights1 = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6};
    std::vector<double> weights1 = brain.getWeights();
    assert(expectedWeights1 == weights1);

    std::vector<double> weights2 = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
    brain.setWeights(weights2);
    std::string pathToWeights2 = "../player/test/resources/testWeights2.txt";
    brain.setPathToWeights(pathToWeights2);
    brain.saveWeights();
    brain.loadWeights();
    assert(brain.getWeights() == weights2);
}

void TestBrain::test_boardToFeatures() {
    Board board;
    Brain b;

    // Just use material weights for this test
    b.setFeatureSet(FeatureSet::MATERIAL);

    // Vanilla case where everyone has all pieces
    std::vector<Feature> features = b.boardToFeatures(board, 1);

    assert(features.size() == FeatureUtils::INDEX_ATTACKING_WEIGHTS_START);

    int i;
    for (i = 0; i < FeatureUtils::NUM_PLAYERS; ++i) {
        // Check no one in check or mate
        assert(features[FeatureUtils::INDEX_CHECK_WEIGHTS_START + i] == 0);
        assert(features[FeatureUtils::INDEX_CHECKMATE_WEIGHTS_START + i] == 0);
    }

    // Everyone should have an even balance of pieces
    assert(features[FeatureUtils::INDEX_MATERIAL_WEIGHTS_START + FeatureUtils::pieceTypeToIndex(PieceType::PAWN)] == 0);
    assert(features[FeatureUtils::INDEX_MATERIAL_WEIGHTS_START + FeatureUtils::pieceTypeToIndex(PieceType::ROOK)] == 0);
    assert(features[FeatureUtils::INDEX_MATERIAL_WEIGHTS_START + FeatureUtils::pieceTypeToIndex(PieceType::WIZARD)] == 0);
    assert(features[FeatureUtils::INDEX_MATERIAL_WEIGHTS_START + FeatureUtils::pieceTypeToIndex(PieceType::BISHOP)] == 0);
    // The pawn distance weights should be balanced
    assert(features[FeatureUtils::INDEX_PAWN_DISTANCE_WEIGHTS_START] == 0);

    // Check partner wizard weights both at 2
    assert(features[FeatureUtils::INDEX_PARTNER_WIZARD_WEIGHTS_START] == 2);
    assert(features[FeatureUtils::INDEX_PARTNER_WIZARD_WEIGHTS_START + 1] == 2);

    // Move queen 4c2 off board, Move queen 1c2 to 4c2
    Position p1c2 ("1c2");
    Position p4c2 ("4c2");
    Position offBoard ("OB");
    Piece* queen1c2 = board.getPieceAtPosition(&p1c2);
    Piece* queen4c2 = board.getPieceAtPosition(&p4c2);
    board.setPiecePosition(queen4c2, &offBoard);
    board.setPiecePosition(queen1c2, &p4c2);
    board.initialiseAttDefVectors();

    std::vector<Feature> features2 = b.boardToFeatures(board, 4);

    // Note that this is from perspective of p4
    // Check p4 is in check
    // Check p4 has no queen
    assert(features2[FeatureUtils::INDEX_CHECK_WEIGHTS_START] == 1);
    assert(features2[FeatureUtils::INDEX_CHECKMATE_WEIGHTS_START] == 0);
    assert(features2[FeatureUtils::pieceTypeToIndex(PieceType::QUEEN)] == -1);
    // Check p1 not in check
    assert(features2[FeatureUtils::INDEX_CHECK_WEIGHTS_START + 3] == 0);
    assert(features2[FeatureUtils::INDEX_CHECKMATE_WEIGHTS_START + 3] == 0);


    // Check that from player 1's perspective, their team has a queen, and the other does not
    std::vector<Feature> features3 = b.boardToFeatures(board, 1);
    assert(features3[FeatureUtils::INDEX_CHECK_WEIGHTS_START] == 0);
    assert(features3[FeatureUtils::INDEX_CHECKMATE_WEIGHTS_START] == 0);
    assert(features3[FeatureUtils::INDEX_CHECK_WEIGHTS_START + 3] == 1);
    assert(features3[FeatureUtils::INDEX_CHECKMATE_WEIGHTS_START + 3] == 0);
    assert(features3[FeatureUtils::pieceTypeToIndex(PieceType::QUEEN)] == 1);

    // Move player 2 wizard off board, move players 1 3 and 5 wizards off board
    Position p1b1 ("1b1");
    Position p2b1 ("2b1");
    Position p3b1 ("3b1");
    Position p5b1 ("5b1");
    board.setPiecePosition(board.getPieceAtPosition(&p1b1), &offBoard);
    board.setPiecePosition(board.getPieceAtPosition(&p2b1), &offBoard);
    board.setPiecePosition(board.getPieceAtPosition(&p3b1), &offBoard);
    board.setPiecePosition(board.getPieceAtPosition(&p5b1), &offBoard);
    board.initialiseAttDefVectors();
    std::vector<Feature> features4 = b.boardToFeatures(board, 1);
    assert(features4[FeatureUtils::INDEX_PARTNER_WIZARD_WEIGHTS_START] == 0);
    assert(features4[FeatureUtils::INDEX_PARTNER_WIZARD_WEIGHTS_START + 1] == 1);

    // Make sure bias weight on
    assert(features4[FeatureUtils::INDEX_BIAS_WEIGHTS_START] == 1);

    // Move pawn at 1c3 to 1c6
    Position p1c3 ("1c3");
    Position p1c6 ("1c6");
    Piece* pawn1c3 = board.getPieceAtPosition(&p1c3);
    board.setPiecePosition(pawn1c3, &p1c6);
    board.initialiseAttDefVectors();
    std::vector<Feature> features5 = b.boardToFeatures(board, 1);
    // Player 1 should now have a slight advantage on pawn distance
    assert(features5[FeatureUtils::INDEX_PAWN_DISTANCE_WEIGHTS_START] == 3);

}

void TestBrain::test_updateWeightsForFeatures() {
    assert(true);
}

void TestBrain::test_randomNumber() {
    Brain b;
    assert(b.getRandomNumber(100) < 100);
    assert(b.getRandomNumber(24) >= 0);
}

void TestBrain::test_sortMovesBasedOnHeuristic() {
    const Position from2 ("1d4");
    const Position to2 ("6a4");

    Move m2(&from2, &to2);

    Position from3 ("2d1");
    Position to3 ("5c4");
    Piece takePiece(&to3, PieceType::PAWN, 3);

    Move m3(&from3, &to3, &takePiece);

    Position to4 ("5c6");
    Piece takePiece2(&to4, PieceType::ROOK, 3);

    Move m4(&from3, &to4, &takePiece2);

    Position offBoard ("OB");
    Piece takePiece3(&to3, PieceType::PAWN, 3);
    Piece replacePiece(&offBoard, PieceType::QUEEN, 2);

    Move m5(&from3, &to3, &takePiece3, &replacePiece);

    std::vector<Move> moves { m3, m4, m5, m2};
    std::vector<Move> expectedOrder {m5, m4, m3, m2};

    Brain b;
    b.setPathToWeights("../weights/wcci/materialCheck.wts");
    b.loadWeights();

    b.sortMovesBasedOnHeuristic(moves);

    assert(moves == expectedOrder);

}

void TestBrain::test_getPlayerOffsetBySeats() {
    assert(Brain::getPlayerOffsetBySeats(1, 0) == 1);
    assert(Brain::getPlayerOffsetBySeats(6, 1) == 1);
    assert(Brain::getPlayerOffsetBySeats(3, 3) == 6);
    assert(Brain::getPlayerOffsetBySeats(4, 3) == 1);
    assert(Brain::getPlayerOffsetBySeats(1, 2) == 3);
    assert(Brain::getPlayerOffsetBySeats(1, 5) == 6);
}

void TestBrain::test_seatsBetweenPlayers() {
    assert(Brain::getSeatsBetweenPlayers(1, 1) == 0);
    assert(Brain::getSeatsBetweenPlayers(1, 5) == 4);
    assert(Brain::getSeatsBetweenPlayers(5, 1) == 2);
    assert(Brain::getSeatsBetweenPlayers(6, 5) == 5);
}

void TestBrain::test_positionIndexesRelativeToPlayerLookup() {

    assert(Brain::positionIndexesRelativeToPlayer[0][0] == 1);
    assert(Brain::positionIndexesRelativeToPlayer[1][0] == 21);
    assert(Brain::positionIndexesRelativeToPlayer[2][0] == 41);
    assert(Brain::positionIndexesRelativeToPlayer[3][0] == 61);
    assert(Brain::positionIndexesRelativeToPlayer[4][0] == 81);
    assert(Brain::positionIndexesRelativeToPlayer[5][0] == 101);

    assert(Brain::positionIndexesRelativeToPlayer[0][120] == 121);
    assert(Brain::positionIndexesRelativeToPlayer[0][125] == 126);
    assert(Brain::positionIndexesRelativeToPlayer[1][120] == 127);
    assert(Brain::positionIndexesRelativeToPlayer[1][125] == 132);
    assert(Brain::positionIndexesRelativeToPlayer[5][120] == 151);
    assert(Brain::positionIndexesRelativeToPlayer[5][125] == 156);

    assert(Brain::positionIndexesRelativeToPlayer[0][135] == 136);
    assert(Brain::positionIndexesRelativeToPlayer[1][135] == 142);
    assert(Brain::positionIndexesRelativeToPlayer[5][135] == 130);

    assert(Brain::positionIndexesRelativeToPlayer[0][156] == 157);
    assert(Brain::positionIndexesRelativeToPlayer[1][156] == 157);
    assert(Brain::positionIndexesRelativeToPlayer[2][156] == 157);
    assert(Brain::positionIndexesRelativeToPlayer[3][156] == 157);
    assert(Brain::positionIndexesRelativeToPlayer[4][156] == 157);
    assert(Brain::positionIndexesRelativeToPlayer[5][156] == 157);

}

void TestBrain::test_naiveAttDef() {
    Board board;
    Brain b;

    b.setFeatureSet(FeatureSet::MATERIAL_NAIVEATTDEF);

    // Move queen1c2 to 1d6
    // It's defending: pawn5b3, pawn1d3, pawn1a3
    // It's attacking: pawn6b3, pawn2d3

    Position p1c2 ("1c2");
    Position p1d6 ("1d6");

    Piece* queen1c2 = board.getPieceAtPosition(&p1c2);

    board.setPiecePosition(queen1c2, &p1d6);

    board.initialiseAttDefVectors();

    const std::vector<Feature>& features1 = b.boardToFeatures(board, 1);
    const std::vector<Feature>& features2 = b.boardToFeatures(board, 2);
    assert(features1.size() == FeatureUtils::INDEX_PIECE_SQUARE_AFTER_NAIVE_WEIGHTS_START);
    assert(features2.size() == FeatureUtils::INDEX_PIECE_SQUARE_AFTER_NAIVE_WEIGHTS_START);

    const int startAttIndex = FeatureUtils::INDEX_ATTACKING_NAIVE_WEIGHTS_START;
    const int startDefIndex = FeatureUtils::INDEX_DEFENDING_NAIVE_WEIGHTS_START;
    // My team's queens are attacking 3 pawns
    assert(features1[startAttIndex + (FeatureUtils::pieceTypeToIndex(PieceType::QUEEN) * FeatureUtils::NUM_UNIQUE_PIECES) + FeatureUtils::PAWN_INDEX] == 3);
    // My team's queens are defending 9 pawns
    assert(features1[startDefIndex + (FeatureUtils::pieceTypeToIndex(PieceType::QUEEN) * FeatureUtils::NUM_UNIQUE_PIECES) + FeatureUtils::PAWN_INDEX] == 9);
    // Enemy team queens are attacking 0 pawns
    assert(features1[startAttIndex + (FeatureUtils::NUM_UNIQUE_PIECES * FeatureUtils::NUM_UNIQUE_PIECES) + (FeatureUtils::pieceTypeToIndex(PieceType::QUEEN) * FeatureUtils::NUM_UNIQUE_PIECES) + FeatureUtils::PAWN_INDEX] == 0);
    // Enemy team queens are defending 9 pawns
    assert(features1[startDefIndex + (FeatureUtils::NUM_UNIQUE_PIECES * FeatureUtils::NUM_UNIQUE_PIECES) + (FeatureUtils::pieceTypeToIndex(PieceType::QUEEN) * FeatureUtils::NUM_UNIQUE_PIECES) + FeatureUtils::PAWN_INDEX] == 9);
    // Swap perspectives, check that it's the other way around now
    assert(features2[startAttIndex + (FeatureUtils::pieceTypeToIndex(PieceType::QUEEN) * FeatureUtils::NUM_UNIQUE_PIECES) + FeatureUtils::PAWN_INDEX] == 0);
    assert(features2[startDefIndex + (FeatureUtils::pieceTypeToIndex(PieceType::QUEEN) * FeatureUtils::NUM_UNIQUE_PIECES) + FeatureUtils::PAWN_INDEX] == 9);
    assert(features2[startAttIndex + (FeatureUtils::NUM_UNIQUE_PIECES * FeatureUtils::NUM_UNIQUE_PIECES) + (FeatureUtils::pieceTypeToIndex(PieceType::QUEEN) * FeatureUtils::NUM_UNIQUE_PIECES) + FeatureUtils::PAWN_INDEX] == 3);
    assert(features2[startDefIndex + (FeatureUtils::NUM_UNIQUE_PIECES * FeatureUtils::NUM_UNIQUE_PIECES) + (FeatureUtils::pieceTypeToIndex(PieceType::QUEEN) * FeatureUtils::NUM_UNIQUE_PIECES) + FeatureUtils::PAWN_INDEX] == 9);

}

void TestBrain::test_attDef() {
    Board board;
    Brain b;

    b.setFeatureSet(FeatureSet::MATERIAL_ATTDEF);

    // Move queen1c2 to 1d6
    // It's defending: pawn5b3, pawn1d3, pawn1a3
    // It's attacking: pawn6b3, pawn2d3

    Position p1c2 ("1c2");
    Position p1d6 ("1d6");

    Piece* queen1c2 = board.getPieceAtPosition(&p1c2);

    board.setPiecePosition(queen1c2, &p1d6);

    board.initialiseAttDefVectors();

    const std::vector<Feature>& features1 = b.boardToFeatures(board, 1);
    assert(features1.size() == FeatureUtils::INDEX_PIECE_SQUARE_WEIGHTS_START);

    const int startAttIndex = FeatureUtils::INDEX_ATTACKING_WEIGHTS_START;
    const int startDefIndex = FeatureUtils::INDEX_DEFENDING_WEIGHTS_START;
//    const int activePlayerOffSetAmount = FeatureUtils::NUM_UNIQUE_PIECES * FeatureUtils::NUM_PLAYERS * FeatureUtils::NUM_UNIQUE_PIECES;
    const int passivePlayerOffSetAmount = FeatureUtils::NUM_UNIQUE_PIECES;
    // My queen is attacking 2 of player 6's pawns
    assert(features1[startAttIndex + (FeatureUtils::pieceTypeToIndex(PieceType::QUEEN) * 3 * FeatureUtils::NUM_UNIQUE_PIECES) + (2 * passivePlayerOffSetAmount) + FeatureUtils::PAWN_INDEX] == 2);
    // My queen is attacking 1 of player 2's pawns
    assert(features1[startAttIndex + (FeatureUtils::pieceTypeToIndex(PieceType::QUEEN) * 3 * FeatureUtils::NUM_UNIQUE_PIECES) + FeatureUtils::PAWN_INDEX] == 1);
    // My queen is defending 2 of my pawns
    assert(features1[startDefIndex + (FeatureUtils::pieceTypeToIndex(PieceType::QUEEN) * 3 * FeatureUtils::NUM_UNIQUE_PIECES) + FeatureUtils::PAWN_INDEX] == 2);
    // My queen is defending 1 of p5's pawns
    assert(features1[startDefIndex + (FeatureUtils::pieceTypeToIndex(PieceType::QUEEN) * 3 * FeatureUtils::NUM_UNIQUE_PIECES) + (2 * passivePlayerOffSetAmount) + FeatureUtils::PAWN_INDEX] == 1);

}

void TestBrain::test_pieceSquare() {
    Board board;
    Brain b;

    b.setFeatureSet(FeatureSet::MATERIAL_ATTDEF_PIECESQUARE);

    // Move queen1c2 to 1d6
    // It's defending: pawn5b3, pawn1d3, pawn1a3
    // It's attacking: pawn6b3, pawn2d3

    Position p1c2 ("1c2");
    Position p6c1 ("6c1");
    Position p1d6 ("1d6");
    Position vortex ("V");

    Piece* queen1c2 = board.getPieceAtPosition(&p1c2);
    Piece* king6c1 = board.getPieceAtPosition(&p6c1);

    board.setPiecePosition(queen1c2, &p1d6);

    board.initialiseAttDefVectors();

    const std::vector<Feature>& features1 = b.boardToFeatures(board, 1);
    const std::vector<Feature>& features2 = b.boardToFeatures(board, 2);

    // Check that relative to player 1, there is a p1 queen at 1d6 (relative/real index 125)
    // Check that relative to player 2, there is a p1 queen at 1d6 (relative index 155)
    int f1Index = FeatureUtils::INDEX_PIECE_SQUARE_WEIGHTS_START + 0 * FeatureUtils::NUM_UNIQUE_PIECES * FeatureUtils::NUM_SQUARES + FeatureUtils::pieceTypeToIndex(PieceType::QUEEN) * FeatureUtils::NUM_SQUARES + 125 - 1;
    Feature f1 = features1[f1Index];
    int f2Index = FeatureUtils::INDEX_PIECE_SQUARE_WEIGHTS_START + 5 * FeatureUtils::NUM_UNIQUE_PIECES * FeatureUtils::NUM_SQUARES + FeatureUtils::pieceTypeToIndex(PieceType::QUEEN) * FeatureUtils::NUM_SQUARES + 155 - 1;
    Feature f2 = features2[f2Index];

    assert(f1 == 1);
    assert(f2 == 1);

    // Check vortex doesn't blow up
    board.setPiecePosition(king6c1, &vortex);
    const std::vector<Feature>& features3 = b.boardToFeatures(board, 1);
    const std::vector<Feature>& features4 = b.boardToFeatures(board, 6);

    assert(features3.size() == FeatureUtils::TOTAL_WEIGHTS);
    assert(features4.size() == FeatureUtils::TOTAL_WEIGHTS);

}

void TestBrain::test_addTargetValToWeightDelta() {
    Board board;
    Brain b;

    b.setFeatureSet(FeatureSet::MATERIAL_ATTDEF);

    // Move queen1c2 to 1d6
    // It's defending: pawn5b3, pawn1d3, pawn1a3
    // It's attacking: pawn6b3, pawn2d3

    Position p1c2 ("1c2");
    Position p1d6 ("1d6");

    Piece* queen1c2 = board.getPieceAtPosition(&p1c2);

    board.setPiecePosition(queen1c2, &p1d6);

    board.initialiseAttDefVectors();

    const std::vector<Feature>& features1 = b.boardToFeatures(board, 1);
    assert(features1.size() == FeatureUtils::INDEX_PIECE_SQUARE_WEIGHTS_START);

    const int startAttIndex = FeatureUtils::INDEX_ATTACKING_WEIGHTS_START;
    const int startDefIndex = FeatureUtils::INDEX_DEFENDING_WEIGHTS_START;
//    const int activePlayerOffSetAmount = FeatureUtils::NUM_UNIQUE_PIECES * FeatureUtils::NUM_PLAYERS * FeatureUtils::NUM_UNIQUE_PIECES;
    const int passivePlayerOffSetAmount = FeatureUtils::NUM_UNIQUE_PIECES;

    const unsigned int idx1 = startAttIndex + (FeatureUtils::pieceTypeToIndex(PieceType::QUEEN) * 3 * FeatureUtils::NUM_UNIQUE_PIECES) + (2 * passivePlayerOffSetAmount) + FeatureUtils::PAWN_INDEX;
    const unsigned int idx2 = startAttIndex + (FeatureUtils::pieceTypeToIndex(PieceType::QUEEN) * 3 * FeatureUtils::NUM_UNIQUE_PIECES) + FeatureUtils::PAWN_INDEX;
    const unsigned int idx3 = startDefIndex + (FeatureUtils::pieceTypeToIndex(PieceType::QUEEN) * 3 * FeatureUtils::NUM_UNIQUE_PIECES) + FeatureUtils::PAWN_INDEX;
    const unsigned int idx4 = startDefIndex + (FeatureUtils::pieceTypeToIndex(PieceType::QUEEN) * 3 * FeatureUtils::NUM_UNIQUE_PIECES) + (2 * passivePlayerOffSetAmount) + FeatureUtils::PAWN_INDEX;
    // My queen is attacking 2 of player 6's pawns
    assert(features1[idx1] == 2);
    // My queen is attacking 1 of player 2's pawns
    assert(features1[idx2] == 1);
    // My queen is defending 2 of my pawns
    assert(features1[idx3] == 2);
    // My queen is defending 1 of p5's pawns
    assert(features1[idx4] == 1);

    // Set some arbitrary weights
    std::vector<double> tempWeights;
    tempWeights.assign(features1.size(), 0);
    b.setWeights(tempWeights);
    b.resetWeightDelta();
    assert(b.getWeightDelta()[idx1] == 0);
    assert(b.getWeightDelta()[idx2] == 0);
    assert(b.getWeightDelta()[idx3] == 0);
    assert(b.getWeightDelta()[idx4] == 0);
    assert(b.getWeights()[idx1] == 0);
    assert(b.getWeights()[idx2] == 0);
    assert(b.getWeights()[idx3] == 0);
    assert(b.getWeights()[idx4] == 0);


    b.addTargetValToWeightDelta(features1, 10.5, 1, board);

    assert(b.getWeightDelta()[idx1] != 0);
    assert(b.getWeightDelta()[idx2] != 0);
    assert(b.getWeightDelta()[idx3] != 0);
    assert(b.getWeightDelta()[idx4] != 0);

    b.applyWeightDelta();
    assert(b.getWeightDelta()[idx1] == 0);
    assert(b.getWeightDelta()[idx2] == 0);
    assert(b.getWeightDelta()[idx3] == 0);
    assert(b.getWeightDelta()[idx4] == 0);
    assert(b.getWeights()[idx1] != 0);
    assert(b.getWeights()[idx2] != 0);
    assert(b.getWeights()[idx3] != 0);
    assert(b.getWeights()[idx4] != 0);
}

void TestBrain::test_kingDist() {
    Brain brain1;
    brain1.setFeatureSet(FeatureSet::MATERIAL_ATTDEF_KINGDIST_PIECE_SQUARE);
    Board board;
    Position p1c2 ("1c2");
    Position p1a2 ("1a2");
    Position p1b4 ("1b4");
    Position p1d2 ("1d2");
    Position p2c1 ("2c1");
    Position p6d1 ("6d1");
    Position p5c1 ("5c1");
    Piece* bishop1a2 = board.getPieceAtPosition(&p1a2);
    const std::vector<Feature>& features1 = brain1.boardToFeatures(board, 1);
    assert(features1.size() == FeatureUtils::TOTAL_WEIGHTS_KING_DIST);
    assert(features1[Brain::getKingDistFeatureIndex(PieceType::QUEEN, 1, 1, 2, &p1c2, &p2c1)] == 1);
    assert(features1[Brain::getKingDistFeatureIndex(PieceType::QUEEN, 1, 1, 2, &p1c2, &p2c1) + 1] == 0);
    assert(features1[Brain::getKingDistFeatureIndex(PieceType::QUEEN, 1, 1, 2, &p1c2, &p2c1) - 1] == 0);
    assert(features1[Brain::getKingDistFeatureIndex(PieceType::BISHOP, 1, 1, 2, &p1d2, &p2c1)] == 1);
    assert(Brain::getKingDistFeatureIndex(PieceType::BISHOP, 1, 1, 2, &p1a2, &p2c1) == -1);

    board.setPiecePosition(bishop1a2, &p1b4);
    const std::vector<Feature>& features2 = brain1.boardToFeatures(board, 1);
    assert(features2[Brain::getKingDistFeatureIndex(PieceType::BISHOP, 1, 1, 2, &p1d2, &p2c1)] == 2);

    Brain brain2;
    brain2.setFeatureSet(FeatureSet::MATERIAL_ATTDEF_KINGDIST);
    brain2.setPathToWeights("../weights/wcci/materialCheck2.wts");
    brain2.loadWeightsAndFillVoid();
    assert(brain2.getWeights().size() == FeatureUtils::INDEX_PIECE_SQUARE_AFTER_KING_DIST_START);
    Board board2;
    const std::vector<Feature>& features3 = brain1.boardToFeatures(board2, 1);
    assert(features3.size() == FeatureUtils::TOTAL_WEIGHTS_KING_DIST);
    assert(features3[Brain::getKingDistFeatureIndex(PieceType::FORTRESS, 1, 6, 5, &p6d1, &p5c1)] == 1);
    double val = brain2.evaluateBoardState(board2, 1);
    assert(val != 100);


}