#include "testLegacyUtils.h"

#include "../LegacyUtils.h"
#include "../../duchess/Board.h"
#include "../../duchess/Position.h"
#include "../../duchess/Piece.h"

#include <iostream>
#include <string>
#include <cassert>

void TestLegacyUtils::runTests() {
    TestLegacyUtils::test_legacyPieceNumberToPieceIndex();
    TestLegacyUtils::test_legacyPieceNumberToOwner();
    TestLegacyUtils::test_legacySquareToPosition();
    TestLegacyUtils::test_positionToLegacyPosIndex();
}

void TestLegacyUtils::test_legacyPieceNumberToPieceIndex() {
    Board b;

    int pawn1a3 = 10;
    int pawn1d3 = 13;
    int pawn2a3 = 25;
    int duchess2 = 18;
    int pawn3c3 = 42;
    int bishop3d2 = 39;
    int knight4 = 50;
    int queen4 = 47;
    int pawn5e3 = 74;
    int pawn6a3 = 85;
    int rook1a1 = 6;
    int rook6e1 = 82;

    Position p1a3 ("1a3");
    Position p1d3 ("1d3");
    Position p2a3 ("2a3");
    Position p2e2 ("2e2");
    Position p3c3 ("3c3");
    Position p3d2 ("3d2");
    Position p4b2 ("4b2");
    Position p4c2 ("4c2");
    Position p5e3 ("5e3");
    Position p6a3 ("6a3");
    Position p1a1 ("1a1");
    Position p6e1 ("6e1");

    assert(LegacyUtils::pieceToModernRelativeIndex(b.getPieceAtPosition(&p1a3), &b) == LegacyUtils::legacyPieceNumberToPieceIndex(pawn1a3));
    assert(LegacyUtils::pieceToModernRelativeIndex(b.getPieceAtPosition(&p1d3), &b) == LegacyUtils::legacyPieceNumberToPieceIndex(pawn1d3));
    assert(LegacyUtils::pieceToModernRelativeIndex(b.getPieceAtPosition(&p2a3), &b) == LegacyUtils::legacyPieceNumberToPieceIndex(pawn2a3));
    assert(LegacyUtils::pieceToModernRelativeIndex(b.getPieceAtPosition(&p2e2), &b) == LegacyUtils::legacyPieceNumberToPieceIndex(duchess2));
    assert(LegacyUtils::pieceToModernRelativeIndex(b.getPieceAtPosition(&p3c3), &b) == LegacyUtils::legacyPieceNumberToPieceIndex(pawn3c3));
    assert(LegacyUtils::pieceToModernRelativeIndex(b.getPieceAtPosition(&p3d2), &b) == LegacyUtils::legacyPieceNumberToPieceIndex(bishop3d2));
    assert(LegacyUtils::pieceToModernRelativeIndex(b.getPieceAtPosition(&p4b2), &b) == LegacyUtils::legacyPieceNumberToPieceIndex(knight4));
    assert(LegacyUtils::pieceToModernRelativeIndex(b.getPieceAtPosition(&p4c2), &b) == LegacyUtils::legacyPieceNumberToPieceIndex(queen4));
    assert(LegacyUtils::pieceToModernRelativeIndex(b.getPieceAtPosition(&p5e3), &b) == LegacyUtils::legacyPieceNumberToPieceIndex(pawn5e3));
    assert(LegacyUtils::pieceToModernRelativeIndex(b.getPieceAtPosition(&p6a3), &b) == LegacyUtils::legacyPieceNumberToPieceIndex(pawn6a3));
    assert(LegacyUtils::pieceToModernRelativeIndex(b.getPieceAtPosition(&p1a1), &b) == LegacyUtils::legacyPieceNumberToPieceIndex(rook1a1));
    assert(LegacyUtils::pieceToModernRelativeIndex(b.getPieceAtPosition(&p6e1), &b) == LegacyUtils::legacyPieceNumberToPieceIndex(rook6e1));
}

void TestLegacyUtils::test_legacyPieceNumberToOwner() {
    assert(LegacyUtils::legacyPieceNumberToOwner(10) == 1);
    assert(LegacyUtils::legacyPieceNumberToOwner(18) == 2);
    assert(LegacyUtils::legacyPieceNumberToOwner(42) == 3);
    assert(LegacyUtils::legacyPieceNumberToOwner(50) == 4);
    assert(LegacyUtils::legacyPieceNumberToOwner(74) == 5);
    assert(LegacyUtils::legacyPieceNumberToOwner(85) == 6);
}

void TestLegacyUtils::test_legacySquareToPosition() {
    Position p1a3 ("1a3");
    Position p1a4 ("1a4");
    Position p2e2 ("2e2");
    Position p2d4 ("2d4");
    Position p3c3 ("3c3");
    Position p3c4 ("3c4");
    Position p4b2 ("4b2");
    Position p4a4 ("4a4");
    Position p5e3 ("5e3");
    Position p5e4 ("5e4");
    Position p6a3 ("6a3");
    Position p6a4 ("6a4");
    Position p1b6 ("1b6");
    Position p1c6 ("1c6");
    Position p2a4 ("2a4");
    Position p3c5 ("3c5");
    Position p2b5 ("2b5");
    Position p3c6 ("3c6");
    Position p1d6 ("1d6");
    Position p2b6 ("2b6");
    Position p6d6 ("6d6");
    Position p1e5 ("1e5");
    Position vortex ("V");
    Position offBoard ("OB");

    int index1a3 = 10;
    int index1a4 = 15;
    int index2e2 = 35;
    int index2d4 = 44;
    int index3c3 = 64;
    int index3c4 = 69;
    int index4b2 = 84;
    int index4a4 = 93;
    int index5e3 = 118;
    int index5e4 = 123;
    int index6a3 = 140;
    int index6a4 = 145;
    int index1b6 = 24;
    int index1c6 = 25;
    int index2a4 = 41;
    int index3c5 = 74;
    int index2b5 = 47;
    int index3c6 = 77;
    int index1d6 = 154;
    int index2b6 = 50;
    int index6d6 = 128;
    int index1e5 = 150;
    int indexVortex = 156;
    int indexOB = 157;

    Position* pos1 = LegacyUtils::legacySquareToPosition(index1a3);
    Position* pos2 = LegacyUtils::legacySquareToPosition(index1a4);
    Position* pos3 = LegacyUtils::legacySquareToPosition(index2e2);
    Position* pos4 = LegacyUtils::legacySquareToPosition(index2d4);
    Position* pos5 = LegacyUtils::legacySquareToPosition(index3c3);
    Position* pos6 = LegacyUtils::legacySquareToPosition(index3c4);
    Position* pos7 = LegacyUtils::legacySquareToPosition(index4b2);
    Position* pos8 = LegacyUtils::legacySquareToPosition(index4a4);
    Position* pos9 = LegacyUtils::legacySquareToPosition(index5e3);
    Position* pos10 = LegacyUtils::legacySquareToPosition(index5e4);
    Position* pos11 = LegacyUtils::legacySquareToPosition(index6a3);
    Position* pos12 = LegacyUtils::legacySquareToPosition(index6a4);
    Position* pos13 = LegacyUtils::legacySquareToPosition(index1b6);
    Position* pos14 = LegacyUtils::legacySquareToPosition(index1c6);
    Position* pos15 = LegacyUtils::legacySquareToPosition(index2a4);
    Position* pos16 = LegacyUtils::legacySquareToPosition(index3c5);
    Position* pos17 = LegacyUtils::legacySquareToPosition(index2b5);
    Position* pos18 = LegacyUtils::legacySquareToPosition(index3c6);
    Position* pos19 = LegacyUtils::legacySquareToPosition(index1d6);
    Position* pos20 = LegacyUtils::legacySquareToPosition(index2b6);
    Position* pos21 = LegacyUtils::legacySquareToPosition(index6d6);
    Position* pos22 = LegacyUtils::legacySquareToPosition(index1e5);
    Position* pos23 = LegacyUtils::legacySquareToPosition(indexVortex);
    Position* pos24 = LegacyUtils::legacySquareToPosition(indexOB);

    assert(*pos1 == p1a3);
    assert(*pos2 == p1a4);
    assert(*pos3 == p2e2);
    assert(*pos4 == p2d4);
    assert(*pos5 == p3c3);
    assert(*pos6 == p3c4);
    assert(*pos7 == p4b2);
    assert(*pos8 == p4a4);
    assert(*pos9 == p5e3);
    assert(*pos10 == p5e4);
    assert(*pos11 == p6a3);
    assert(*pos12 == p6a4);
    assert(*pos13 == p1b6);
    assert(*pos14 == p1c6);
    assert(*pos15 == p2a4);
    assert(*pos16 == p3c5);
    assert(*pos17 == p2b5);
    assert(*pos18 == p3c6);
    assert(*pos19 == p1d6);
    assert(*pos20 == p2b6);
    assert(*pos21 == p6d6);
    assert(*pos22 == p1e5);
    assert(*pos23 == vortex);
    assert(*pos24 == offBoard);

    delete pos1;
    delete pos2;
    delete pos3;
    delete pos4;
    delete pos5;
    delete pos6;
    delete pos7;
    delete pos8;
    delete pos9;
    delete pos10;
    delete pos11;
    delete pos12;
    delete pos13;
    delete pos14;
    delete pos15;
    delete pos16;
    delete pos17;
    delete pos18;
    delete pos19;
    delete pos20;
    delete pos21;
    delete pos22;
    delete pos23;
    delete pos24;
}

void TestLegacyUtils::test_positionToLegacyPosIndex() {
    Position p1a3 ("1a3");
    Position p1a4 ("1a4");
    Position p2e2 ("2e2");
    Position p2d4 ("2d4");
    Position p3c3 ("3c3");
    Position p3c4 ("3c4");
    Position p4b2 ("4b2");
    Position p4a4 ("4a4");
    Position p5e3 ("5e3");
    Position p5e4 ("5e4");
    Position p6a3 ("6a3");
    Position p6a4 ("6a4");

    Position p1b6 ("1b6");
    Position p1c6 ("1c6");
    Position p2a4 ("2a4");
    Position p3c5 ("3c5");
    Position p2b5 ("2b5");
    Position p3c6 ("3c6");
    Position p1d6 ("1d6");
    Position p2b6 ("2b6");
    Position p6d6 ("6d6");
    Position p1e5 ("1e5");
    Position vortex ("V");
    Position offBoard ("OB");


    int index1a3 = 10;
    int index1a4 = 15;
    int index2e2 = 35;
    int index2d4 = 44;
    int index3c3 = 64;
    int index3c4 = 69;
    int index4b2 = 84;
    int index4a4 = 93;
    int index5e3 = 118;
    int index5e4 = 123;
    int index6a3 = 140;
    int index6a4 = 145;

    int index1b6 = 24;
    int index1c6 = 25;
    int index2a4 = 41;
    int index3c5 = 74;
    int index2b5 = 47;
    int index3c6 = 77;
    int index1d6 = 154;
    int index2b6 = 50;
    int index6d6 = 128;
    int index1e5 = 150;
    int indexVortex = 156;
    int indexOB = 157;


    assert(LegacyUtils::positionToLegacyPosIndex(&p1a3) == index1a3);
    assert(LegacyUtils::positionToLegacyPosIndex(&p1a4) == index1a4);
    assert(LegacyUtils::positionToLegacyPosIndex(&p2e2) == index2e2);
    assert(LegacyUtils::positionToLegacyPosIndex(&p2d4) == index2d4);
    assert(LegacyUtils::positionToLegacyPosIndex(&p3c3) == index3c3);
    assert(LegacyUtils::positionToLegacyPosIndex(&p3c4) == index3c4);
    assert(LegacyUtils::positionToLegacyPosIndex(&p4b2) == index4b2);
    assert(LegacyUtils::positionToLegacyPosIndex(&p4a4) == index4a4);
    assert(LegacyUtils::positionToLegacyPosIndex(&p5e3) == index5e3);
    assert(LegacyUtils::positionToLegacyPosIndex(&p5e4) == index5e4);
    assert(LegacyUtils::positionToLegacyPosIndex(&p6a3) == index6a3);
    assert(LegacyUtils::positionToLegacyPosIndex(&p6a4) == index6a4);

    assert(LegacyUtils::positionToLegacyPosIndex(&p1b6) == index1b6);
    assert(LegacyUtils::positionToLegacyPosIndex(&p1c6) == index1c6);
    assert(LegacyUtils::positionToLegacyPosIndex(&p2a4) == index2a4);
    assert(LegacyUtils::positionToLegacyPosIndex(&p3c5) == index3c5);
    assert(LegacyUtils::positionToLegacyPosIndex(&p2b5) == index2b5);
    assert(LegacyUtils::positionToLegacyPosIndex(&p3c6) == index3c6);
    assert(LegacyUtils::positionToLegacyPosIndex(&p1d6) == index1d6);
    assert(LegacyUtils::positionToLegacyPosIndex(&p2b6) == index2b6);
    assert(LegacyUtils::positionToLegacyPosIndex(&p6d6) == index6d6);
    assert(LegacyUtils::positionToLegacyPosIndex(&p1e5) == index1e5);
    assert(LegacyUtils::positionToLegacyPosIndex(&vortex) == indexVortex);
    assert(LegacyUtils::positionToLegacyPosIndex(&offBoard) == indexOB);
}
