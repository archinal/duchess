#include <assert.h>

#include "testPiece.h"

#include "../Piece.h"
#include "../Position.h"
#include "../PieceType.h"
#include "../Board.h"
#include "../MoveVector.h"

void TestPiece::runTests() {
    TestPiece::test_constructors();
    TestPiece::test_accessorsRevealData();
    TestPiece::test_updatePosition();
    TestPiece::test_equals();
    TestPiece::test_moveVectorManipulators();
}

void TestPiece::test_constructors() {
    Position pos ("1c2");
    const short owner = 1;
    Piece piece (&pos, PieceType::PAWN, owner);
}

void TestPiece::test_accessorsRevealData() {
    Position pos ("1c2");
    const short owner = 1;
    Piece piece (&pos, PieceType::PAWN, owner);

    assert(*piece.getPosition() == pos);
    assert(piece.getOwner() == owner);
    assert(piece.getType() == PieceType::PAWN);
}

void TestPiece::test_updatePosition() {
    Position pos1 ("1c2");
    const short owner = 1;
    Piece piece (&pos1, PieceType::PAWN, owner);

    assert(*piece.getPosition() == pos1);

    Position pos2 ("V");
    piece.updatePosition(&pos2);
    assert(*piece.getPosition() == pos2);
}

void TestPiece::test_equals() {
    Position pos1 ("1c2");
    const short owner1 = 1;
    Piece piece1 (&pos1, PieceType::PAWN, owner1);

    Position pos2 ("1b2");
    const short owner2 = 3;
    Piece piece (&pos2, PieceType::KING, owner2);

    assert(pos1 == pos1);
    assert(!(pos1 != pos1));
    assert(pos2 == pos2);
    assert(pos1 != pos2);
}

void TestPiece::test_moveVectorManipulators() {
    Board b;

    Position pos1 ("1a3");
    Position pos2 ("1e2");
    Piece* piece1 = b.getPieceAtPosition(&pos1);
    Piece* piece2 = b.getPieceAtPosition(&pos2);

    Position pos7 ("6d1"); // Player 6 fortress
    Piece* piece3 = b.getPieceAtPosition(&pos7);

    // Note this is not a true move vector in the sense that the positions listed do not really connect these pieces
    Position pos3 ("1c3");
    Position pos4 ("1d3");
    Position pos5 ("1e3");

    std::vector<Position*> squares = {&pos1, &pos2, &pos3, &pos4, &pos5};
    MoveVector* mv1 = new MoveVector (piece1, piece2, squares);
    MoveVector* mv2 = new MoveVector (piece1, piece3, squares);
    MoveVector* mv3 = new MoveVector (piece2, piece1, squares);
    MoveVector* mv4 = new MoveVector (piece3, piece1, squares);

    assert(piece1->getActiveAttackingVectors().size() == 0);
    assert(piece1->getActiveDefendingVectors().size() == 0);
    assert(piece1->getPassiveAttackingVectors().size() == 0);
    assert(piece1->getPassiveDefendingVectors().size() == 0);

    piece1->addMoveVector(mv1);

    assert(piece1->getActiveAttackingVectors().size() == 0);
    assert(piece1->getActiveDefendingVectors().size() == 1);
    assert(piece1->getPassiveAttackingVectors().size() == 0);
    assert(piece1->getPassiveDefendingVectors().size() == 0);

    piece1->addMoveVector(mv2);

    assert(piece1->getActiveAttackingVectors().size() == 1);
    assert(piece1->getActiveDefendingVectors().size() == 1);
    assert(piece1->getPassiveAttackingVectors().size() == 0);
    assert(piece1->getPassiveDefendingVectors().size() == 0);

    piece1->addMoveVector(mv3);

    assert(piece1->getActiveAttackingVectors().size() == 1);
    assert(piece1->getActiveDefendingVectors().size() == 1);
    assert(piece1->getPassiveAttackingVectors().size() == 0);
    assert(piece1->getPassiveDefendingVectors().size() == 1);

    piece1->addMoveVector(mv4);

    assert(piece1->getActiveAttackingVectors().size() == 1);
    assert(piece1->getActiveDefendingVectors().size() == 1);
    assert(piece1->getPassiveAttackingVectors().size() == 1);
    assert(piece1->getPassiveDefendingVectors().size() == 1);

    piece1->eraseMoveVector(mv4);

    assert(piece1->getActiveAttackingVectors().size() == 1);
    assert(piece1->getActiveDefendingVectors().size() == 1);
    assert(piece1->getPassiveAttackingVectors().size() == 0);
    assert(piece1->getPassiveDefendingVectors().size() == 1);

    piece1->clearAllMoveVectors();

    assert(piece1->getActiveAttackingVectors().size() == 0);
    assert(piece1->getActiveDefendingVectors().size() == 0);
    assert(piece1->getPassiveAttackingVectors().size() == 0);
    assert(piece1->getPassiveDefendingVectors().size() == 0);

    // Delete this as we cheekily bypassed its destructor by manually erasing it
    delete mv4;
}