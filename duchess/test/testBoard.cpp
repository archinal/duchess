#include "testBoard.h"
#include "../Position.h"
#include "../PositionUtils.h"
#include "../Piece.h"
#include "../PieceType.h"
#include "../Board.h"

#include <algorithm>
#include <cassert>
#include <vector>
#include <iostream>

void TestBoard::runTests() {
    TestBoard::test_constructor();
    TestBoard::test_getPieceAtPosition();
    TestBoard::test_getPosition();
    TestBoard::test_getPiecesForPlayer();
    TestBoard::test_isPieceWizardAdjacent();
    TestBoard::test_doesSquareContain();
    TestBoard::test_getAccessibleSquares();
    TestBoard::test_getAccessibleSquaresOnVortex();
    TestBoard::test_setPiecePosition();
    TestBoard::test_initialiseAttDefVectors();
    TestBoard::test_getAttackingLineOnKingThroughPosition();
    TestBoard::test_getLegalMoves();
    TestBoard::test_applyMoveDeNovo();
    TestBoard::test_applyMoveCumulative();
    TestBoard::test_applyMoveWithCheckDeNovo();
    TestBoard::test_applyMoveWithCheckCumulative();
    TestBoard::test_getLinesAttackingSquare();
    TestBoard::test_getActiveLinesFromVortex();
    TestBoard::test_undoMove();
    TestBoard::test_moveVectorsThroughVortex();
}

void TestBoard::test_constructor() {
    Board b;
}

void TestBoard::test_getPieceAtPosition() {
    Board b;
    const Position pos1 ("1a1");
    const Position pos2 ("3e3");
    const Position pos3 ("6c2");
    const Position pos4 ("V");
    Piece* piece1 = b.getPieceAtPosition(&pos1);
    Piece* piece2 = b.getPieceAtPosition(&pos2);
    Piece* piece3 = b.getPieceAtPosition(&pos3);
    Piece* piece4 = b.getPieceAtPosition(&pos4);

    assert(piece1->getOwner() == 1);
    assert(piece1->getType() == PieceType::ROOK);
    assert(*(piece1->getPosition()) == pos1);

    assert(piece2->getOwner() == 3);
    assert(piece2->getType() == PieceType::PAWN);
    assert(*(piece2->getPosition()) == pos2);

    assert(piece3->getOwner() == 6);
    assert(piece3->getType() == PieceType::QUEEN);
    assert(*(piece3->getPosition()) == pos3);

    assert(piece4 == nullptr);
}

void TestBoard::test_getPosition() {
    Board b;

    const Position pos1 ("1a1");
    const Position pos2 ("3e3");
    const Position pos3 ("6c2");
    const Position pos4 ("V");

    assert(*(b.getPosition(PositionUtils::positionToBoardIndexSixPlayer(&pos1))) == pos1);
    assert(*(b.getPosition(PositionUtils::positionToBoardIndexSixPlayer(&pos2))) == pos2);
    assert(*(b.getPosition(PositionUtils::positionToBoardIndexSixPlayer(&pos3))) == pos3);
    assert(*(b.getPosition(PositionUtils::positionToBoardIndexSixPlayer(&pos4))) == pos4);
}

void TestBoard::test_getPiecesForPlayer() {
    Board b;
    const short owner = 2;
    std::vector<Piece*> pieces = b.getPiecesForPlayer(owner);
    assert(pieces.size() == 15);
    assert(pieces[0]->getType() == PieceType::PAWN);
    assert(pieces[13]->getType() == PieceType::FORTRESS);
    assert(pieces[14]->getType() == PieceType::ROOK);
    assert(pieces[0]->getOwner() == owner);
}

void TestBoard::test_isPieceWizardAdjacent() {
    Board b;

    Position pos1 ("1a1");
    Position pos2 ("2c1");
    Position pos3 ("6b2");
    Position pos4 ("1b1");

    Position pos5 ("1b3");
    Position pos6 ("4e3");

    Position pos7 ("1a2");
    Position pos8 ("1c2");

    Piece* p1 = b.getPieceAtPosition(&pos1);
    Piece* p2 = b.getPieceAtPosition(&pos2); // Yes
    Piece* p3 = b.getPieceAtPosition(&pos3); // Yes
    Piece* p4 = b.getPieceAtPosition(&pos4); // Yes

    Piece* p5 = b.getPieceAtPosition(&pos5); // No
    Piece* p6 = b.getPieceAtPosition(&pos6); // No

    Piece* p7 = b.getPieceAtPosition(&pos7); // Yes
    Piece* p8 = b.getPieceAtPosition(&pos8); // Yes

    assert(b.isPieceWizardAdjacent(p1));
    assert(b.isPieceWizardAdjacent(p2));
    assert(b.isPieceWizardAdjacent(p3));
    assert(b.isPieceWizardAdjacent(p4));

    assert(!b.isPieceWizardAdjacent(p5));
    assert(!b.isPieceWizardAdjacent(p6));

    assert(b.isPieceWizardAdjacent(p7));
    assert(b.isPieceWizardAdjacent(p8));
}

void TestBoard::test_doesSquareContain() {
    Board b;

    Position pos1 ("1a1");
    Position pos2 ("2c1");
    Position pos3 ("6c5");

    Piece* p1 = b.getPieceAtPosition(&pos1); // Yes
    Piece* p2 = b.getPieceAtPosition(&pos2); // Yes

    assert(b.doesSquareContainPiece(&pos1));
    assert(b.doesSquareContainPiece(&pos2));
    assert(!b.doesSquareContainPiece(&pos3));

    assert(b.doesSquareContainEnemy(&pos2, p1));
    assert(b.doesSquareContainEnemy(&pos1, p2));
    assert(!b.doesSquareContainEnemy(&pos1, p1));
    assert(!b.doesSquareContainEnemy(&pos2, p2));

    assert(b.doesSquareContainAlly(&pos1, p1));
    assert(b.doesSquareContainAlly(&pos2, p2));
    assert(!b.doesSquareContainAlly(&pos1, p2));
    assert(!b.doesSquareContainAlly(&pos2, p1));
}

void TestBoard::test_getAccessibleSquares() {
    Board b;

    Position pos1 ("1a3");
    Position pos2 ("1a4");

    Position pos3 ("2e4");

    Piece* p1 = b.getPieceAtPosition(&pos1); // Pawn 1

    // PAWN
    std::vector<Position*> squares1 = b.getAccessibleSquares(p1);
    assert(squares1.size() == 1);
    assert(*(squares1[0]) == pos2);

    // Magically move the pawn to 2e4 for testing purposes
    b.setPiecePosition(p1, &pos3);
    std::vector<Position*> squares2 = b.getAccessibleSquares(p1);
    assert(std::find(squares2.begin(), squares2.end(), b.getPosition(34)) != squares2.end());
    assert(std::find(squares2.begin(), squares2.end(), b.getPosition(39)) != squares2.end());
    assert(std::find(squares2.begin(), squares2.end(), b.getPosition(129)) != squares2.end());
    assert(squares2.size() == 3);

    // Move it back
    b.setPiecePosition(p1, &pos1);

    // KNIGHT
    Position knightHome ("1b2");
    Piece* knight = b.getPieceAtPosition(&knightHome);
    std::vector<Position*> squares3 = b.getAccessibleSquares(knight);
    assert(std::find(squares3.begin(), squares3.end(), b.getPosition(16)) != squares3.end());
    assert(std::find(squares3.begin(), squares3.end(), b.getPosition(18)) != squares3.end());
    assert(squares3.size() == 2);

    // Magically move the pawn to 2e4 for testing purposes
    b.setPiecePosition(knight, &pos3);
    std::vector<Position*> squares4 = b.getAccessibleSquares(knight);
    assert(std::find(squares4.begin(), squares4.end(), b.getPosition(33)) != squares4.end());
    assert(std::find(squares4.begin(), squares4.end(), b.getPosition(29)) != squares4.end());
    assert(std::find(squares4.begin(), squares4.end(), b.getPosition(127)) != squares4.end());
    assert(std::find(squares4.begin(), squares4.end(), b.getPosition(131)) != squares4.end());
    assert(std::find(squares4.begin(), squares4.end(), b.getPosition(17)) != squares4.end());
    assert(squares4.size() == 5);
    // Move it back
    b.setPiecePosition(knight, &knightHome);

    // BISHOP
    Position bishopHome ("1a2");
    Piece* bishop = b.getPieceAtPosition(&bishopHome);
    std::vector<Position*> squares5 = b.getAccessibleSquares(bishop);
    assert(squares5.size() == 0);

    b.setPiecePosition(bishop, &pos3);
    std::vector<Position*> squares6 = b.getAccessibleSquares(bishop);
    assert(std::find(squares6.begin(), squares6.end(), b.getPosition(34)) != squares6.end());
    assert(std::find(squares6.begin(), squares6.end(), b.getPosition(128)) != squares6.end());
    assert(std::find(squares6.begin(), squares6.end(), b.getPosition(130)) != squares6.end());
    assert(std::find(squares6.begin(), squares6.end(), b.getPosition(136)) != squares6.end());
    assert(std::find(squares6.begin(), squares6.end(), b.getPosition(144)) != squares6.end());
    assert(std::find(squares6.begin(), squares6.end(), b.getPosition(56)) != squares6.end());
    assert(std::find(squares6.begin(), squares6.end(), b.getPosition(16)) != squares6.end());
    assert(squares6.size() == 7);
    // Move it back
    b.setPiecePosition(bishop, &bishopHome);

    // ROOK
    Position rookHome ("1a1");
    Piece* rook = b.getPieceAtPosition(&rookHome);
    std::vector<Position*> squares7 = b.getAccessibleSquares(rook);
    assert(squares7.size() == 0);

    b.setPiecePosition(rook, &pos3);
    std::vector<Position*> squares8 = b.getAccessibleSquares(rook);
    assert(std::find(squares8.begin(), squares8.end(), b.getPosition(35)) != squares8.end());
    assert(std::find(squares8.begin(), squares8.end(), b.getPosition(36)) != squares8.end());
    assert(std::find(squares8.begin(), squares8.end(), b.getPosition(37)) != squares8.end());
    assert(std::find(squares8.begin(), squares8.end(), b.getPosition(38)) != squares8.end());
    assert(std::find(squares8.begin(), squares8.end(), b.getPosition(39)) != squares8.end());
    assert(std::find(squares8.begin(), squares8.end(), b.getPosition(129)) != squares8.end());
    assert(std::find(squares8.begin(), squares8.end(), b.getPosition(132)) != squares8.end());
    assert(std::find(squares8.begin(), squares8.end(), b.getPosition(121)) != squares8.end());
    assert(std::find(squares8.begin(), squares8.end(), b.getPosition(122)) != squares8.end());
    assert(std::find(squares8.begin(), squares8.end(), b.getPosition(123)) != squares8.end());
    assert(std::find(squares8.begin(), squares8.end(), b.getPosition(116)) != squares8.end());
    assert(std::find(squares8.begin(), squares8.end(), b.getPosition(111)) != squares8.end());
    assert(squares8.size() == 12);
    // Move it back
    b.setPiecePosition(rook, &rookHome);

    // With WIZARD
    // Move ally wizard out to near enemy squares
    Position wizardHome ("3b1");
    Piece* wizard = b.getPieceAtPosition(&wizardHome);
    b.setPiecePosition(wizard, &pos3);

    Position kingHome ("1c1");
    Piece* king = b.getPieceAtPosition(&kingHome);
    std::vector<Position*> squares9 = b.getAccessibleSquares(king);
    assert(std::find(squares9.begin(), squares9.end(), b.getPosition(35)) != squares9.end());
    assert(std::find(squares9.begin(), squares9.end(), b.getPosition(34)) != squares9.end());
    assert(std::find(squares9.begin(), squares9.end(), b.getPosition(39)) != squares9.end());
    assert(std::find(squares9.begin(), squares9.end(), b.getPosition(128)) != squares9.end());
    assert(std::find(squares9.begin(), squares9.end(), b.getPosition(129)) != squares9.end());
    assert(std::find(squares9.begin(), squares9.end(), b.getPosition(16)) != squares9.end());
    assert(squares9.size() == 6);
}

void TestBoard::test_getAccessibleSquaresOnVortex() {
    Board b;
    Position p1a1 ("1a1");
    Position p1a2 ("1a2");
    Position p1c2 ("1c2");
    Position vortex ("V");

    Piece* rook1a1 = b.getPieceAtPosition(&p1a1);
    Piece* bishop1a2 = b.getPieceAtPosition(&p1a2);
    Piece* queen1c2 = b.getPieceAtPosition(&p1c2);

    b.setPiecePosition(rook1a1, &vortex);
    const std::vector<Position*> rookAccessible = b.getAccessibleSquares(rook1a1);
    assert(rookAccessible.size() == 21);
    b.setPiecePosition(rook1a1, &p1a1);

    b.setPiecePosition(bishop1a2, &vortex);
    const std::vector<Position*> bishopAccessible = b.getAccessibleSquares(bishop1a2);
    assert(bishopAccessible.size() == 12);
    b.setPiecePosition(bishop1a2, &p1a2);

    b.setPiecePosition(queen1c2, &vortex);
    const std::vector<Position*> queenAccessible = b.getAccessibleSquares(queen1c2);
    assert(queenAccessible.size() == 12 + 21);
    b.setPiecePosition(queen1c2, &p1c2);

}

void TestBoard::test_setPiecePosition() {
    Board b;

    Position pos1 ("1a3");
    Position dest ("1a5");

    Piece* p1 = b.getPieceAtPosition(&pos1); // Pawn
    b.setPiecePosition(p1, &dest);

    assert(b.getPieceAtPosition(&dest) == p1);
    assert(b.getPieceAtPosition(&pos1) == nullptr);
    assert(*(p1->getPosition()) == dest);
}

void TestBoard::test_initialiseAttDefVectors() {
    Board b;

    Position pos1 ("1a3");
    Position pos2 ("1b3");
    Position pos3 ("1d3");
    Position pos4 ("1c1");

    Piece* p1 = b.getPieceAtPosition(&pos1); // Pawn 1
    Piece* p2 = b.getPieceAtPosition(&pos2); // Pawn 2
    Piece* p3 = b.getPieceAtPosition(&pos3); // Pawn 3
    Piece* p4 = b.getPieceAtPosition(&pos4); // King

    b.initialiseAttDefVectors();

    assert(p1->getActiveDefendingVectors().size() == 1);
    assert(p1->getPassiveDefendingVectors().size() == 0);

    assert(p2->getActiveDefendingVectors().size() == 2);
    assert(p2->getPassiveDefendingVectors().size() == 2);

    assert(p3->getActiveDefendingVectors().size() == 2);
    assert(p3->getPassiveDefendingVectors().size() == 3);

    assert(p4->getActiveDefendingVectors().size() == 5);
    assert(p4->getPassiveDefendingVectors().size() == 5);

}

void TestBoard::TestBoard::test_getAttackingLineOnKingThroughPosition() {
    Board b;
    Position pos1 ("1c1");
    Piece* king = b.getPieceAtPosition(&pos1);
    Position kingDest ("2d5");
    Position enemyPawnPos ("2b3");
    Piece* enemyPawn = b.getPieceAtPosition(&enemyPawnPos);
    Position farAway ("5d5");

    // Move player 2's pawn at 2b3 away (say to flap 5)
    b.setPiecePosition(enemyPawn, &farAway);
    // Move king to 2d4
    b.setPiecePosition(king, &kingDest);

    // Set t_position as 2c3
    Position targetPos1 ("2c4");
    Position enemyBishopPos ("2a2");

    // Make sure we bring up bishop
    std::vector<Position*> attackingLine1 = b.getAttackingLineOnKingThroughPosition(&targetPos1, king->getOwner());
    // DEBUG: Print
    assert(*attackingLine1[0] == *(king->getPosition()));
    assert(*attackingLine1[3] == enemyBishopPos);
    assert(attackingLine1.size() == 4);

    // Now set t_position as 2d3
    Position targetPos2 ("2d3");
    // Make sure we don't find an attacking vector through t_position
    std::vector<Position*> attackingLine2 = b.getAttackingLineOnKingThroughPosition(&targetPos2, king->getOwner());
    assert(attackingLine2.size() == 0);

}

void TestBoard::test_getLegalMoves() {
    Board b;
    b.initialiseAttDefVectors();
    const std::vector<Move> legalMoves = b.getLegalMoves(1);

    for (auto const& move : legalMoves) {
        assert(b.isLegalMove(move, 1));
    }

    Position pawn1 ("1a3");
    Position pawn2 ("1b3");
    Position pawn3 ("1c3");
    Position pawn4 ("1d3");
    Position pawn5 ("1e3");
    Position knight ("1b2");
    Position duchess ("1e2");

    Position pawnDest1 ("1a4");
    Position pawnDest2 ("1b4");
    Position pawnDest3 ("1c4");
    Position pawnDest4 ("1d4");
    Position pawnDest5 ("1e4");
    Position knightDest1 ("1c4");
    Position knightDest2 ("1a4");
    Position duchessDest ("1d4");

    Move move1 (&pawn1, &pawnDest1);
    Move move2 (&pawn2, &pawnDest2);
    Move move3 (&pawn3, &pawnDest3);
    Move move4 (&pawn4, &pawnDest4);
    Move move5 (&pawn5, &pawnDest5);
    Move move6 (&knight, &knightDest1);
    Move move7 (&knight, &knightDest2);
    Move move8 (&duchess, &duchessDest);

    assert(std::find(legalMoves.begin(), legalMoves.end(), move1) != legalMoves.end());
    assert(std::find(legalMoves.begin(), legalMoves.end(), move2) != legalMoves.end());
    assert(std::find(legalMoves.begin(), legalMoves.end(), move3) != legalMoves.end());
    assert(std::find(legalMoves.begin(), legalMoves.end(), move4) != legalMoves.end());
    assert(std::find(legalMoves.begin(), legalMoves.end(), move5) != legalMoves.end());
    assert(std::find(legalMoves.begin(), legalMoves.end(), move6) != legalMoves.end());
    assert(std::find(legalMoves.begin(), legalMoves.end(), move7) != legalMoves.end());
    assert(std::find(legalMoves.begin(), legalMoves.end(), move8) != legalMoves.end());

    assert(legalMoves.size() == 8);
}

void TestBoard::test_applyMoveDeNovo() {
    Board b1;
    b1.initialiseAttDefVectors();

    Position pawn1 ("1a3");
    Position pawn2 ("2e3");
    Position pawn3 ("3d3");
    Position pawn4 ("4d3");
    Position pawn5 ("5d3");
    Position pawn6 ("6d3");
    Position pawnDest1 ("1a4");
    Position pawnDest2 ("2e4");
    Position pawnDest3 ("3d4");
    Position pawnDest4 ("4d4");
    Position pawnDest5 ("5d4");
    Position pawnDest6 ("6d4");
    Position knight1 ("2b2");
    Position bishop1 ("2a2");
    Position knight1Dest ("2c4");
    Move move1 (&pawn1, &pawnDest1);
    Move move2 (&pawn2, &pawnDest2);
    Move move3 (&pawn3, &pawnDest3);
    Move move4 (&pawn4, &pawnDest4);
    Move move5 (&pawn5, &pawnDest5);
    Move move6 (&pawn6, &pawnDest6);
    Move move7 (&pawnDest1, &pawnDest2, b1.getPieceAtPosition(&pawnDest2));
    Move move8 (&knight1, &knight1Dest);
    Piece* pawn1Piece = b1.getPieceAtPosition(&pawn1);
    Piece* pawn2Piece = b1.getPieceAtPosition(&pawn2);
    Piece* bishop1Piece = b1.getPieceAtPosition(&bishop1);

    b1.applyMoveDeNovo(&move1);
    assert(*(pawn1Piece->getPosition()) == pawnDest1);
    assert(pawn1Piece->getActiveAttackingVectors().size() == 0);
    assert(pawn1Piece->getActiveDefendingVectors().size() == 1);
    assert(pawn1Piece->getPassiveDefendingVectors().size() == 2);
    assert(pawn1Piece->getPassiveAttackingVectors().size() == 0);
    b1.applyMoveDeNovo(&move2);

    assert(pawn1Piece->getActiveAttackingVectors().size() == 1);
    assert(pawn1Piece->getActiveDefendingVectors().size() == 1);
    assert(pawn1Piece->getPassiveDefendingVectors().size() == 2);
    assert(pawn1Piece->getPassiveAttackingVectors().size() == 1);

    assert(pawn2Piece->getActiveAttackingVectors().size() == 1);
    assert(pawn2Piece->getActiveDefendingVectors().size() == 1);
    assert(pawn2Piece->getPassiveAttackingVectors().size() == 1);
    assert(pawn2Piece->getPassiveDefendingVectors().size() == 1);

    b1.applyMoveDeNovo(&move3);
    b1.applyMoveDeNovo(&move4);
    b1.applyMoveDeNovo(&move5);
    b1.applyMoveDeNovo(&move6);
    // Pawn 1 takes pawn 2
    b1.applyMoveDeNovo(&move7);

    // Check that pawn 2 is now off board
    assert(pawn2Piece->getPosition()->isOffBoard());

    // Check that moving the rook away now lets the queen defend the bishop
    assert(bishop1Piece->getPassiveDefendingVectors().size() == 3);
    b1.applyMoveDeNovo(&move8);
    assert(bishop1Piece->getPassiveDefendingVectors().size() == 4);
}

void TestBoard::test_applyMoveCumulative() {
    Board b1;
    b1.initialiseAttDefVectors();

    Position pawn1 ("1a3");
    Position pawn2 ("2e3");
    Position pawn3 ("3d3");
    Position pawn4 ("4d3");
    Position pawn5 ("5d3");
    Position pawn6 ("6d3");
    Position pawnDest1 ("1a4");
    Position pawnDest2 ("2e4");
    Position pawnDest3 ("3d4");
    Position pawnDest4 ("4d4");
    Position pawnDest5 ("5d4");
    Position pawnDest6 ("6d4");
    Position knight1 ("2b2");
    Position bishop1 ("2a2");
    Position knight1Dest ("2c4");
    Move move1 (&pawn1, &pawnDest1);
    Move move2 (&pawn2, &pawnDest2);
    Move move3 (&pawn3, &pawnDest3);
    Move move4 (&pawn4, &pawnDest4);
    Move move5 (&pawn5, &pawnDest5);
    Move move6 (&pawn6, &pawnDest6);
    Move move7 (&pawnDest1, &pawnDest2, b1.getPieceAtPosition(&pawnDest2));
    Move move8 (&knight1, &knight1Dest);
    Piece* pawn1Piece = b1.getPieceAtPosition(&pawn1);
    Piece* pawn2Piece = b1.getPieceAtPosition(&pawn2);
    Piece* bishop1Piece = b1.getPieceAtPosition(&bishop1);

    b1.applyMoveCumulative(&move1);

    assert(*(pawn1Piece->getPosition()) == pawnDest1);
    assert(pawn1Piece->getActiveAttackingVectors().size() == 0);
    assert(pawn1Piece->getActiveDefendingVectors().size() == 1);
    assert(pawn1Piece->getPassiveDefendingVectors().size() == 2);
    assert(pawn1Piece->getPassiveAttackingVectors().size() == 0);
    b1.applyMoveCumulative(&move2);

    assert(pawn1Piece->getActiveAttackingVectors().size() == 1);
    assert(pawn1Piece->getActiveDefendingVectors().size() == 1);
    assert(pawn1Piece->getPassiveDefendingVectors().size() == 2);
    assert(pawn1Piece->getPassiveAttackingVectors().size() == 1);

    assert(pawn2Piece->getActiveAttackingVectors().size() == 1);
    assert(pawn2Piece->getActiveDefendingVectors().size() == 1);
    assert(pawn2Piece->getPassiveAttackingVectors().size() == 1);
    assert(pawn2Piece->getPassiveDefendingVectors().size() == 1);

    b1.applyMoveCumulative(&move3);
    b1.applyMoveCumulative(&move4);
    b1.applyMoveCumulative(&move5);
    b1.applyMoveCumulative(&move6);
    // Pawn 1 takes pawn 2
    b1.applyMoveCumulative(&move7);

    // Check that pawn 2 is now off board
    assert(pawn2Piece->getPosition()->isOffBoard());

    // Check that moving the rook away now lets the queen defend the bishop
    assert(bishop1Piece->getPassiveDefendingVectors().size() == 3);
    b1.applyMoveCumulative(&move8);
    assert(bishop1Piece->getPassiveDefendingVectors().size() == 4);
}

void TestBoard::test_applyMoveWithCheckDeNovo() {
    Board b;
    // Set up all necessary positions and piece pointers
    Position p1a1 ("1a1");
    Position p1a2 ("1a2");
    Position p1b3 ("1b3");
    Position p1b4 ("1b4");
    Position p1c1 ("1c1");
    Position p1c2 ("1c2");
    Position p1c3 ("1c3");
    Position p1b5 ("1b5");
    Position p1b2 ("1b2");

    Position p2a3 ("2a3");
    Position p2a4 ("2a4");
    Position p2b2 ("2b2");
    Position p2d3 ("2d3");
    Position p2d4 ("2d4");
    Position p2c1 ("2c1");
    Position p2c2 ("2c2");
    Position p2c3 ("2c3");
    Position p2c4 ("2c4");
    Position p2c5 ("2c5");

    Position p3c3 ("3c3");
    Position p3c4 ("3c4");
    Position p3c5 ("3c5");
    Position p3c6 ("3c6");

    Position p4c3 ("4c3");
    Position p4c4 ("4c4");
    Position p4c5 ("4c5");
    Position p4c6 ("4c6");
    Position p4d6 ("4d6");

    Position p5c3 ("5c3");
    Position p5c4 ("5c4");
    Position p5c5 ("5c5");
    Position p5c6 ("5c6");
    Position p5d6 ("5d6");

    Position p6c2 ("6c2");
    Position p6c3 ("6c3");
    Position p6c4 ("6c4");
    Position p6c5 ("6c5");

    Position vortex ("V");

    Piece* queen1 = b.getPieceAtPosition(&p1c2);
    Piece* queen2 = b.getPieceAtPosition(&p2c2);
//    Piece* queen6 = b.getPieceAtPosition(&p6c2);
//    Piece* king1 = b.getPieceAtPosition(&p1c1);
//    Piece* king2 = b.getPieceAtPosition(&p2c1);
//    Piece* knight1 = b.getPieceAtPosition(&p1b2);
//    Piece* knight2 = b.getPieceAtPosition(&p2b2);
    Piece* pawn13 = b.getPieceAtPosition(&p1c3);

    // Create move objects
    Move m1 (&p1b3, &p1b4);
    Move m2 (&p2d3, &p2d4);
    Move m3 (&p3c3, &p3c4);
    Move m4 (&p4c3, &p4c4);
    Move m5 (&p5c3, &p5c4);
    Move m6 (&p6c3, &p6c4);

    Move m7 (&p1c2, &p2c2, queen2);
    Move m8Fake (&p2a3, &p2a4);
    Move m8 (&p2b2, &p2c2, queen1);

    Move m9 (&p3c4, &p3c5);
    Move m10 (&p4c4, &p4c5);
    Move m11 (&p5c4, &p5c5);
    Move m12 (&p6c2, &p6c3);
    Move m13 (&p1b4, &p1b5);
    Move m14 (&p2c3, &p2c4);
    Move m15 (&p3c5, &p3c6);
    Move m16 (&p4c5, &p4c6);
    Move m17 (&p5c5, &p5c6);

    Move m18(&p6c3, &p1c3, pawn13);
    Move m19(b.getPosition(&p1b2), b.getPosition(&p1c2));
    Move m19Fake1(b.getPosition(&p1c1), b.getPosition(&p1c2));
    Move m19Fake2(b.getPosition(&p1a1), b.getPosition(&p1a2));

    Move m20(&p2c4, &p2c5);
    Move m21(&p3c6, &vortex);
    Move m22(&p4c6, &p4d6);
    Move m23(&p5c6, &p5d6);
    Move m24(&p6c4, &p6c5);
    Move m25Fake(b.getPosition(&p1c1), b.getPosition(&p1b2));


    // Do the work with assertions
    b.applyMoveDeNovo(&m1);
    b.applyMoveDeNovo(&m2);
    b.applyMoveDeNovo(&m3);
    b.applyMoveDeNovo(&m4);
    b.applyMoveDeNovo(&m5);
    b.applyMoveDeNovo(&m6);

    // Take enemy queen
    b.applyMoveDeNovo(&m7);
    // P2 Should be in check now
    std::vector<Move> legalMoves1 = b.getLegalMoves(2);
    assert(queen2->getPosition()->isOffBoard());
    assert(b.isPlayerInCheck(2));
    assert(std::find(legalMoves1.begin(), legalMoves1.end(), m8Fake) == legalMoves1.end());
    assert(std::find(legalMoves1.begin(), legalMoves1.end(), m8) != legalMoves1.end());

    // Take Player 1's queen
    b.applyMoveDeNovo(&m8);
    assert(!b.isPlayerInCheck(2));
    assert(queen1->getPosition()->isOffBoard());

    b.applyMoveDeNovo(&m9);
    b.applyMoveDeNovo(&m10);
    b.applyMoveDeNovo(&m11);
    b.applyMoveDeNovo(&m12);
    b.applyMoveDeNovo(&m13);
    b.applyMoveDeNovo(&m14);
    b.applyMoveDeNovo(&m15);
    b.applyMoveDeNovo(&m16);
    b.applyMoveDeNovo(&m17);

    // Queen 6 takes pawn13 to put P1 in check
    b.applyMoveDeNovo(&m18);

    assert(b.isPlayerInCheck(1));
    assert(pawn13->getPosition()->isOffBoard());
    std::vector<Move> legalMoves2 = b.getLegalMoves(1);
    assert(!b.isLegalMove(m19Fake1, 1));
    assert(!b.isLegalMove(m19Fake2, 1));
    assert(b.isLegalMove(m19, 1));
    // Check we can teleport our knight across to block the path
    assert(std::find(legalMoves2.begin(), legalMoves2.end(), m19) != legalMoves2.end());
    // Check we can't just move a pawn
    assert(std::find(legalMoves2.begin(), legalMoves2.end(), m19Fake2) == legalMoves2.end());
    // Check king can't just move up and stand in front of queen
    assert(std::find(legalMoves2.begin(), legalMoves2.end(), m19Fake1) == legalMoves2.end());

    b.applyMoveDeNovo(&m19);
    assert(!b.isPlayerInCheck(1));

    b.applyMoveDeNovo(&m20);
    b.applyMoveDeNovo(&m21);
    b.applyMoveDeNovo(&m22);
    b.applyMoveDeNovo(&m23);
    b.applyMoveDeNovo(&m24);
    // Check king can't move diagonal adjacent to queen
    std::vector<Move> legalMoves3 = b.getLegalMoves(1);
    assert(!b.isLegalMove(m25Fake, 1));
    assert(std::find(legalMoves3.begin(), legalMoves3.end(), m25Fake) == legalMoves3.end());

}

void TestBoard::test_applyMoveWithCheckCumulative() {
    Board b;
    // Set up all necessary positions and piece pointers
    Position p1a1 ("1a1");
    Position p1a2 ("1a2");
    Position p1b3 ("1b3");
    Position p1b4 ("1b4");
    Position p1c1 ("1c1");
    Position p1c2 ("1c2");
    Position p1c3 ("1c3");
    Position p1b5 ("1b5");
    Position p1b2 ("1b2");

    Position p2a3 ("2a3");
    Position p2a4 ("2a4");
    Position p2b2 ("2b2");
    Position p2d3 ("2d3");
    Position p2d4 ("2d4");
    Position p2c1 ("2c1");
    Position p2c2 ("2c2");
    Position p2c3 ("2c3");
    Position p2c4 ("2c4");
    Position p2c5 ("2c5");

    Position p3c3 ("3c3");
    Position p3c4 ("3c4");
    Position p3c5 ("3c5");
    Position p3c6 ("3c6");

    Position p4c3 ("4c3");
    Position p4c4 ("4c4");
    Position p4c5 ("4c5");
    Position p4c6 ("4c6");
    Position p4d6 ("4d6");

    Position p5c3 ("5c3");
    Position p5c4 ("5c4");
    Position p5c5 ("5c5");
    Position p5c6 ("5c6");
    Position p5d6 ("5d6");

    Position p6c2 ("6c2");
    Position p6c3 ("6c3");
    Position p6c4 ("6c4");
    Position p6c5 ("6c5");

    Position vortex ("V");

    Piece* queen1 = b.getPieceAtPosition(&p1c2);
    Piece* queen2 = b.getPieceAtPosition(&p2c2);
//    Piece* queen6 = b.getPieceAtPosition(&p6c2);
//    Piece* king1 = b.getPieceAtPosition(&p1c1);
//    Piece* king2 = b.getPieceAtPosition(&p2c1);
//    Piece* knight1 = b.getPieceAtPosition(&p1b2);
//    Piece* knight2 = b.getPieceAtPosition(&p2b2);
    Piece* pawn13 = b.getPieceAtPosition(&p1c3);

    // Create move objects
    Move m1 (&p1b3, &p1b4);
    Move m2 (&p2d3, &p2d4);
    Move m3 (&p3c3, &p3c4);
    Move m4 (&p4c3, &p4c4);
    Move m5 (&p5c3, &p5c4);
    Move m6 (&p6c3, &p6c4);

    Move m7 (&p1c2, &p2c2, queen2);
    Move m8Fake (&p2a3, &p2a4);
    Move m8 (&p2b2, &p2c2, queen1);

    Move m9 (&p3c4, &p3c5);
    Move m10 (&p4c4, &p4c5);
    Move m11 (&p5c4, &p5c5);
    Move m12 (&p6c2, &p6c3);
    Move m13 (&p1b4, &p1b5);
    Move m14 (&p2c3, &p2c4);
    Move m15 (&p3c5, &p3c6);
    Move m16 (&p4c5, &p4c6);
    Move m17 (&p5c5, &p5c6);

    Move m18(&p6c3, &p1c3, pawn13);
    Move m19(&p1b2, &p1c2);
    Move m19Fake1(&p1c1, &p1c2);
    Move m19Fake2(&p1a1, &p1a2);

    Move m20(&p2c4, &p2c5);
    Move m21(&p3c6, &vortex);
    Move m22(&p4c6, &p4d6);
    Move m23(&p5c6, &p5d6);
    Move m24(&p6c4, &p6c5);
    Move m25Fake(&p1c1, &p1b2);


    // Do the work with assertions
    b.applyMoveCumulative(&m1);
    b.applyMoveCumulative(&m2);
    b.applyMoveCumulative(&m3);
    b.applyMoveCumulative(&m4);
    b.applyMoveCumulative(&m5);
    b.applyMoveCumulative(&m6);

    // Take enemy queen
    b.applyMoveCumulative(&m7);
    // P2 Should be in check now
    std::vector<Move> legalMoves1 = b.getLegalMoves(2);
    assert(queen2->getPosition()->isOffBoard());
    assert(b.isPlayerInCheck(2));
    assert(std::find(legalMoves1.begin(), legalMoves1.end(), m8Fake) == legalMoves1.end());
    assert(std::find(legalMoves1.begin(), legalMoves1.end(), m8) != legalMoves1.end());

    // Take Player 1's queen
    b.applyMoveCumulative(&m8);
    assert(!b.isPlayerInCheck(2));
    assert(queen1->getPosition()->isOffBoard());

    b.applyMoveCumulative(&m9);
    b.applyMoveCumulative(&m10);
    b.applyMoveCumulative(&m11);
    b.applyMoveCumulative(&m12);
    b.applyMoveCumulative(&m13);
    b.applyMoveCumulative(&m14);
    b.applyMoveCumulative(&m15);
    b.applyMoveCumulative(&m16);
    b.applyMoveCumulative(&m17);

    // Queen 6 takes pawn13 to put P1 in check
    b.applyMoveCumulative(&m18);

    assert(b.isPlayerInCheck(1));
    assert(pawn13->getPosition()->isOffBoard());
    std::vector<Move> legalMoves2 = b.getLegalMoves(1);
    // Check we can teleport our knight across to block the path
    assert(std::find(legalMoves2.begin(), legalMoves2.end(), m19) != legalMoves2.end());
    // Check we can't just move a pawn
    assert(std::find(legalMoves2.begin(), legalMoves2.end(), m19Fake2) == legalMoves2.end());
    // Check king can't just move up and stand in front of queen
    assert(std::find(legalMoves2.begin(), legalMoves2.end(), m19Fake1) == legalMoves2.end());

    b.applyMoveCumulative(&m19);
    assert(!b.isPlayerInCheck(1));

    b.applyMoveCumulative(&m20);
    b.applyMoveCumulative(&m21);
    b.applyMoveCumulative(&m22);
    b.applyMoveCumulative(&m23);
    b.applyMoveCumulative(&m24);
    // Check king can't move diagonal adjacent to queen
    std::vector<Move> legalMoves3 = b.getLegalMoves(1);
    assert(std::find(legalMoves3.begin(), legalMoves3.end(), m25Fake) == legalMoves3.end());

}

void TestBoard::test_getLinesAttackingSquare() {
    Board b;
    // Case 1: Remove pawn13 and pawn14, check on 1c3, should turn up Queen1, Bishop12

    Position vortex ("V");
    Position p1c3 ("1c3");
    Position p1a5 ("2e5");
    Position p1b4 ("1b4");

    Position p1e5 ("1e5");
    Position p1d5 ("1d5");
    Position p1c4 ("1c4");
    Position p1c5 ("1c5");
    Position p1c6 ("1c6");
    Position p1b5 ("2e6");
    Position p2d6 ("2d6");

    Position p1d3 ("1d3");
    Position p1c2 ("1c2");
    Position p1d1 ("1d1");
    Position p1d2 ("1d2");
    Position p1e2 ("1e2");
    Position p6a4 ("6a4");
    Position p6a3 ("6a3");
    Position p6a2 ("6a2");
    Position p6a1 ("6a1");
    const BoardIndex offBoardIndex = 0;

    Piece* pawn13 = b.getPieceAtPosition(&p1c3);
    Piece* pawn14 = b.getPieceAtPosition(&p1d3);
    Piece* queen1 = b.getPieceAtPosition(&p1c2);
    Piece* bishop2 = b.getPieceAtPosition(&p1d2);
    Piece* duchess1 = b.getPieceAtPosition(&p1e2);
    Piece* fortess1 = b.getPieceAtPosition(&p1d1);
    Piece* pawn61 = b.getPieceAtPosition(&p6a3);
    Piece* bishop62 = b.getPieceAtPosition(&p6a2);
    Piece* rook61 = b.getPieceAtPosition(&p6a1);

    b.setPiecePosition(pawn13, offBoardIndex);
    b.setPiecePosition(pawn14, offBoardIndex);

    std::vector<std::deque<Position*>> attackingLines1 = b.getLinesAttackingSquare(&p1c3);
    std::deque<Position*> line1 {queen1->getPosition(), &p1c3};
    std::deque<Position*> line2 {bishop2->getPosition(), &p1c3};
    std::deque<Position*> line3 {duchess1->getPosition(), &p1c3};
    std::deque<Position*> line4 {fortess1->getPosition(), &p1c3};

    assert(attackingLines1.size() == 4);
    assert(std::find(attackingLines1.begin(), attackingLines1.end(), line1) != attackingLines1.end());
    assert(std::find(attackingLines1.begin(), attackingLines1.end(), line2) != attackingLines1.end());
    assert(std::find(attackingLines1.begin(), attackingLines1.end(), line3) != attackingLines1.end());
    assert(std::find(attackingLines1.begin(), attackingLines1.end(), line4) != attackingLines1.end());

    // Case 2: Now remove pawn61 and bishop61, should be attacked by rook61 and bishop12 at 1a5 (2e5)
    b.setPiecePosition(pawn61, offBoardIndex);
    b.setPiecePosition(bishop62, offBoardIndex);

    std::vector<std::deque<Position*>> attackingLines2 = b.getLinesAttackingSquare(&p1a5);
    std::deque<Position*> line5 {rook61->getPosition(), b.getPosition(&p6a2), b.getPosition(&p6a3), b.getPosition(&p6a4),
                                 b.getPosition(&p1e5), b.getPosition(&p1d5), b.getPosition(&p1c5), b.getPosition(&p1b5),
                                 &p1a5};
    std::deque<Position*> line6 {bishop2->getPosition(), b.getPosition(&p1c3), b.getPosition(&p1b4), &p1a5};

    assert(attackingLines2.size() == 2);
    assert(std::find(attackingLines2.begin(), attackingLines2.end(), line5) != attackingLines2.end());
    assert(std::find(attackingLines2.begin(), attackingLines2.end(), line6) != attackingLines2.end());

    // Now check along vortex
    b.setPiecePosition(bishop2, &p1a5);

    std::vector<std::deque<Position*>> attackingLines3 = b.getLinesAttackingSquare(&vortex);
    // Bishop at 1a5 to vortex
    std::deque<Position*> line7 {bishop2->getPosition(), b.getPosition(&p2d6), &vortex};
    // Queen at 1c2 to vortex
    std::deque<Position*> line8 {queen1->getPosition(), b.getPosition(&p1c3), b.getPosition(&p1c4), b.getPosition(&p1c5), b.getPosition(&p1c6), &vortex};

    assert(attackingLines3.size() == 2);
    assert(std::find(attackingLines3.begin(), attackingLines3.end(), line7) != attackingLines3.end());
    assert(std::find(attackingLines3.begin(), attackingLines3.end(), line8) != attackingLines3.end());

    // Check queen doesn't register as attacking/defending itself on the vortex
    Board b2;
    b2.setPiecePosition(b2.getPieceAtPosition(&p1c2), &vortex);
    std::vector<std::deque<Position*>> linesAttackingVortex = b2.getLinesAttackingSquare(&vortex);
    assert(linesAttackingVortex.size() == 0);
}

void TestBoard::test_undoMove() {
    Board b;

    // Set up an interesting situation
    Position p1c2 ("1c2");
    Position p1b3 ("1b3");
    Position p2d6 ("2d6");
    Position p2e6 ("2e6");
    Position p2d2 ("2d2");
    Position p3e1 ("3e1");
    Position p3d4 ("3d4");
    Position p4c2 ("4c2");
    Position p4c5 ("4c5");
    Position vortex ("V");
    Position offBoard ("OB");

    Piece* queen1 = b.getPieceAtPosition(&p1c2);
    Piece* pawn12 = b.getPieceAtPosition(&p1b3);
    Piece* bishop22 = b.getPieceAtPosition(&p2d2);
    Piece* rook32 = b.getPieceAtPosition(&p3e1);
    Piece* queen4 = b.getPieceAtPosition(&p4c2);

    // Queen1 (1c2) off board
    b.setPiecePosition(queen1, &offBoard);
    // Pawn12 (1b3) diagonal to vortex (1b6/2d6)
    b.setPiecePosition(pawn12, &p2d6);
    // Bishop22 (2d2) on vortex (V)
    b.setPiecePosition(bishop22, &vortex);
    // Rook 32 (3e1) at 3e4 so it's defending pawn
    b.setPiecePosition(rook32, &p3d4);
    // Queen4 (4c2) at 4c5 so it's attacking rook
    b.setPiecePosition(queen4, &p4c5);

    // Move: pawn12 takes bishop22 onto vortex and is replaced by queen1
    Move move1 (pawn12->getPosition(), bishop22->getPosition(), bishop22, queen1);
    Move move2 (pawn12->getPosition(), &p2e6);
    Move move3 (bishop22->getPosition(), pawn12->getPosition(), pawn12);
    Move move4 (queen4->getPosition(), rook32->getPosition(), rook32);

    std::vector<Move> moves {move1, move2, move3, move4};

    for (auto const& move : moves) {

        b.initialiseAttDefVectors();

        // Get move vectors before change
        std::vector <std::string> moveVectorStringsPre;
        // 2D array of player -> piece -> move vectors
        // For each piece we store its move vectors as a string
        short i, j;
        std::vector <std::vector<std::string>> pieceMoveVectorsPre;
        for (i = 1; i <= 6; ++i) {
            std::vector <std::string> vec;
            for (j = 0; j < 15; ++j) {
                vec.push_back(b.getPiecesForPlayer(i)[j]->getMoveVectorsAsString());
            }
            pieceMoveVectorsPre.push_back(vec);
        }

        // CHECK DENOVO
        // Make changes, undo
        b.applyMoveDeNovo(&move);
        b.undoMoveDeNovo(&move);

        // Get move vectors after change/undo
        std::vector <std::string> moveVectorStringsPost1;
        // 2D array of player -> piece -> move vectors
        // For each piece we store its move vectors as a string
        std::vector <std::vector<std::string>> pieceMoveVectorsPost1;
        for (i = 1; i <= 6; ++i) {
            std::vector <std::string> vec;
            for (j = 0; j < 15; ++j) {
                vec.push_back(b.getPiecesForPlayer(i)[j]->getMoveVectorsAsString());
            }
            pieceMoveVectorsPost1.push_back(vec);
        }

        // Check they're equal
        bool error = false;
        for (i = 0; i < 6; ++i) {
            for (j = 0; j < 15; ++j) {
                std::string expected = pieceMoveVectorsPre[i][j];
                std::string real = pieceMoveVectorsPost1[i][j];
                if (expected != real) {
                    std::cout << "For piece: " << b.getPiecesForPlayer(i + 1)[j]->toString() << std::endl;
                    std::cout << "Expected:" << std::endl;
                    std::cout << expected << std::endl;
                    std::cout << "Real:" << std::endl;
                    std::cout << real << std::endl;
                    error = true;
                }
            }
        }
        assert(!error);


        b.initialiseAttDefVectors();

        // CHECK CUMULATIVE
        // Make changes, undo
        b.applyMoveCumulative(&move);

        b.undoMoveCumulative(&move);

        // Get move vectors after change/undo
        std::vector <std::string> moveVectorStringsPost2;
        // 2D array of player -> piece -> move vectors
        // For each piece we store its move vectors as a string
        std::vector <std::vector<std::string>> pieceMoveVectorsPost2;
        for (i = 1; i <= 6; ++i) {
            std::vector <std::string> vec;
            for (j = 0; j < 15; ++j) {
                vec.push_back(b.getPiecesForPlayer(i)[j]->getMoveVectorsAsString());
            }
            pieceMoveVectorsPost2.push_back(vec);
        }

        // Check they're equal
        error = false;
        for (i = 0; i < 6; ++i) {
            for (j = 0; j < 15; ++j) {
                std::string expected = pieceMoveVectorsPre[i][j];
                std::string real = pieceMoveVectorsPost2[i][j];
                if (expected != real) {
                    std::cout << "For piece: " << b.getPiecesForPlayer(i + 1)[j]->toString() << std::endl;
                    std::cout << "Expected:" << std::endl;
                    std::cout << expected << std::endl;
                    std::cout << "Real:" << std::endl;
                    std::cout << real << std::endl;
                    error = true;
                }
            }
        }
        assert(!error);
    }

}

void TestBoard::test_getActiveLinesFromVortex() {
    Board b;

    // Set up an interesting situation
    Position p1a1 ("1a1");
    Position p1c2 ("1c2");
    Position p2c3 ("2c3");
    Position p2e5 ("2e5");
    Position p1a2 ("1a2");
    Position p2d2 ("2d2");
    Position vortex ("V");
    Position offBoard ("OB");

    Piece* queen1 = b.getPieceAtPosition(&p1c2);
    Piece* pawn23 = b.getPieceAtPosition(&p2c3);
    Piece* bishop11 = b.getPieceAtPosition(&p1a2);
    Piece* bishop22 = b.getPieceAtPosition(&p2d2);
    Piece* rook1 = b.getPieceAtPosition(&p1a1);


    b.setPiecePosition(queen1, &vortex);
    b.setPiecePosition(pawn23, &offBoard);
    b.setPiecePosition(bishop22, &p2e5);

    b.initialiseAttDefVectors();


    assert(queen1->getActiveAttackingVectors().size() == 4);
    assert(queen1->getActiveDefendingVectors().size() == 3);

    b.setPiecePosition(queen1, &offBoard);
    b.setPiecePosition(bishop11, &vortex);

    b.initialiseAttDefVectors();

    assert(bishop11->getActiveAttackingVectors().size() == 1);
    assert(bishop11->getActiveDefendingVectors().size() == 0);

    b.setPiecePosition(bishop11, &offBoard);
    b.setPiecePosition(rook1, &vortex);

    b.initialiseAttDefVectors();
    assert(rook1->getActiveAttackingVectors().size() == 3);
    assert(rook1->getActiveDefendingVectors().size() == 3);

}

void TestBoard::test_moveVectorsThroughVortex() {
    Board board;
    Position p1c2 ("1c2");
    Position p1d6 ("1d6");
    Position p2d6 ("2d6");

    Piece* queen1c2 = board.getPieceAtPosition(&p1c2);

    board.setPiecePosition(queen1c2, &p1d6);
    board.initialiseAttDefVectors();
    assert(queen1c2->getActiveAttackingVectors().size() == 3);

    board.setPiecePosition(queen1c2, &p2d6);
    board.initialiseAttDefVectors();
    assert(queen1c2->getActiveAttackingVectors().size() == 3);
}