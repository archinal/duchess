#include "testMoveVector.h"
#include "../Position.h"
#include "../Piece.h"
#include "../PieceType.h"
#include "../MoveVector.h"
#include "../Board.h"

#include <vector>
#include <cassert>

void TestMoveVector::runTests() {
    TestMoveVector::test_constructorAndAccessors();
    TestMoveVector::test_doesCrossOver();
    TestMoveVector::test_doesInvolve();
    TestMoveVector::test_registerDeregister();
}

void TestMoveVector::test_constructorAndAccessors() {
    Board b;

    Position pos1 ("1a3");
    Position pos2 ("1e2");
    Piece* piece1 = b.getPieceAtPosition(&pos1);
    Piece* piece2 = b.getPieceAtPosition(&pos2);

    // Note this is not a true move vector in the sense that the positions listed do not really connect these pieces
    Position pos3 ("1c3");
    Position pos4 ("1d3");
    Position pos5 ("1e3");

    std::vector<Position*> squares = {&pos1, &pos2, &pos3, &pos4, &pos5};
    MoveVector mv (piece1, piece2, squares);

    assert(mv.getActivePiece() == piece1);
    assert(mv.getPassivePiece() == piece2);
    assert(mv.getActivePieceType() == piece1->getType());
    assert(mv.getPassivePieceType() == piece2->getType());
    assert(mv.getPositions() == squares);
    assert(mv.getMoveVectorType() == MoveVectorType::DEFENDING);
}

void TestMoveVector::test_doesCrossOver() {
    Board b;

    Position pos1 ("1a3");
    Position pos2 ("1e2");
    Piece* piece1 = b.getPieceAtPosition(&pos1);
    Piece* piece2 = b.getPieceAtPosition(&pos2);

    // Note this is not a true move vector in the sense that the positions listed do not really connect these pieces
    Position pos3 ("1c3");
    Position pos4 ("1d3");
    Position pos5 ("1e3");

    std::vector<Position*> squares = {&pos1, &pos2, &pos3, &pos4, &pos5};
    MoveVector mv (piece1, piece2, squares);

    Position pos6 ("1d3");
    Position pos7 ("6d3");

    assert(mv.doesCrossOver(&pos1));
    assert(mv.doesCrossOver(&pos6));
    assert(!mv.doesCrossOver(&pos7));
}

void TestMoveVector::test_doesInvolve() {
    Board b;

    Position pos1 ("1a3");
    Position pos2 ("1e2");
    Piece* piece1 = b.getPieceAtPosition(&pos1);
    Piece* piece2 = b.getPieceAtPosition(&pos2);

    // Note this is not a true move vector in the sense that the positions listed do not really connect these pieces
    Position pos3 ("1c3");
    Position pos4 ("1d3");
    Position pos5 ("1e3");

    std::vector<Position*> squares = {&pos1, &pos2, &pos3, &pos4, &pos5};
    MoveVector mv (piece1, piece2, squares);

    Position pos7 ("6d1"); // Player 6 fortress
    Piece* piece3 = b.getPieceAtPosition(&pos7);

    assert(mv.doesInvolve(piece1));
    assert(mv.doesInvolve(piece2));
    assert(!mv.doesInvolve(piece3));

}

void TestMoveVector::test_registerDeregister() {
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

    mv1->registerSelf();
    assert(piece1->getActiveDefendingVectors()[0] == mv1);
    assert(piece2->getPassiveDefendingVectors()[0] == mv1);

    mv2->registerSelf();
    assert(piece1->getActiveAttackingVectors()[0] == mv2);
    assert(piece3->getPassiveAttackingVectors()[0] == mv2);

    mv3->registerSelf();
    assert(piece1->getPassiveDefendingVectors()[0] == mv3);
    assert(piece2->getActiveDefendingVectors()[0] == mv3);

    mv4->registerSelf();
    assert(piece1->getPassiveAttackingVectors()[0] == mv4);
    assert(piece3->getActiveAttackingVectors()[0] == mv4);

    mv4->deregisterSelf();
    assert(piece1->getPassiveAttackingVectors().size() == 0);
    assert(piece3->getActiveAttackingVectors().size() == 0);

    // Check that clearing all ones related to piece1 removes them from his friends too
    piece1->clearAllMoveVectors();
    assert(piece3->getActiveAttackingVectors().size() == 0);
    assert(piece2->getActiveDefendingVectors().size() == 0);
    assert(piece3->getPassiveAttackingVectors().size() == 0);
    assert(piece2->getPassiveDefendingVectors().size() == 0);

    // Delete mv4 since we manually deregistered it, rather than leaving it to Piece::clearAllMoveVectors();
    delete mv4;

}