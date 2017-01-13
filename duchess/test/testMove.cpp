#include "testMove.h"
#include "../Move.h"
#include "../Piece.h"
#include "../PieceType.h"
#include "../Position.h"

#include <cassert>

void TestMove::runTests() {
    TestMove::test_constructorsAndGetters();
    TestMove::test_equals();
    TestMove::test_lessThan();
}

void TestMove::test_constructorsAndGetters() {
    Move m1;

    assert(m1.isNull());
    assert(m1.getFromPosition() == nullptr);
    assert(m1.getToPosition() == nullptr);
    assert(m1.getReplacePiece() == nullptr);
    assert(m1.getPieceTaken() == nullptr);

    const Position from2 ("1d4");
    const Position to2 ("6a4");

    Move m2(&from2, &to2);
    assert(*(m2.getFromPosition()) == from2);
    assert(*(m2.getToPosition()) == to2);
    assert(!m2.isNull());
    assert(m2.getReplacePiece() == nullptr);
    assert(m2.getPieceTaken() == nullptr);

    Position from3 ("2d1");
    Position to3 ("5c4");
    Position offBoard ("OB");
    Piece takePiece(&to3, PieceType::KNIGHT, 3);
    Piece replacePiece(&offBoard, PieceType::QUEEN, 2);

    Move m3(&from3, &to3, &takePiece, &replacePiece);
    assert(*(m3.getFromPosition()) == from3);
    assert(*(m3.getToPosition()) == to3);
    assert(m3.getPieceTaken() == &takePiece);
    assert(m3.getReplacePiece() == &replacePiece);
    assert(!m3.isNull());
}

void TestMove::test_equals() {
    Move m1;

    const Position from2 ("1d4");
    const Position to2 ("6a4");
    Move m2(&from2, &to2);

    Position from3 ("2d1");
    Position to3 ("5c4");
    Position offBoard ("OB");
    Piece takePiece(&to3, PieceType::KNIGHT, 3);
    Piece replacePiece(&offBoard, PieceType::QUEEN, 2);
    Move m3(&from3, &to3, &takePiece, &replacePiece);

    Move m4(&from3, &to3, &takePiece, &replacePiece, true);

    Move m5;

    assert(m1 == m1);
    assert(m1 == m5);
    assert(m1 != m2);
    assert(m1 != m3);
    assert(m1 != m4);

    assert(m3 != m4);
    assert(m3 != m2);

    assert(m2 == m2);
    assert(m3 == m3);
    assert(m4 == m4);
};

void TestMove::test_lessThan() {
    Move m1;

    const Position from2 ("1d4");
    const Position to2 ("6a4");

    Move m2(&from2, &to2);

    Position from3 ("2d1");
    Position to3 ("5c4");
    Position offBoard ("OB");
    Piece takePiece(&to3, PieceType::KNIGHT, 3);
    Piece replacePiece(&offBoard, PieceType::QUEEN, 2);

    Move m3(&from3, &to3, &takePiece, &replacePiece);

    assert(m1 < m2);
    assert(m2 < m3);
    assert(m1 < m3);

}