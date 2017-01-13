#include <assert.h>

#include "testPosition.h"

#include "../Position.h"

void TestPosition::runTests() {
    TestPosition::test_accessorsRevealData();
    TestPosition::test_toStringConvertsToString();
    TestPosition::test_constructors();
    TestPosition::test_equalityOperatorWorks();
    TestPosition::test_sanitizedConstructor();
}

void TestPosition::test_accessorsRevealData() {
    short flap = 1;
    char row = 'a';
    short column = 2;

    Position pos1 (flap, row, column);

    assert(pos1.getFlap() == flap);
    assert(pos1.getRow() == row);
    assert(pos1.getColumn() == column);
}

void TestPosition::test_toStringConvertsToString() {
    short flap = 1;
    char row = 'a';
    short column = 2;

    Position pos1 (flap, row, column);

    Position pos2("OB");
    Position pos3("V");

    assert(pos1.toString() == "1a2");
    assert(pos2.toString() == "OB");
    assert(pos3.toString() == "V");
}

void TestPosition::test_constructors() {
    Position p1 (4, 'c', 5);
    assert(p1.getFlap() == 4);
    assert(p1.getRow() == 'c');
    assert(p1.getColumn() == 5);
    assert(!p1.isOffBoard());
    assert(!p1.isVortex());

    Position p2 ("2c1");
    assert(p2.getFlap() == 2);
    assert(p2.getRow() == 'c');
    assert(p2.getColumn() == 1);
    assert(!p2.isOffBoard());
    assert(!p2.isVortex());

    Position offBoard ("OB");
    assert(offBoard.isOffBoard());
    assert(!offBoard.isVortex());

    Position vortex ("V");
    assert(!vortex.isOffBoard());
    assert(vortex.isVortex());
}

void TestPosition::test_equalityOperatorWorks() {
    // Clearly same
    Position p1 ("1b2");
    Position p2 ("1b2");

    Position p3 ("4d2");
    Position p4 ("4d2");

    assert(p1 == p2);
    assert(p3 == p4);
    assert(p1 != p3);

    // Same by different names
    Position p5("1b5");
    Position p6("2e6");

    Position p7("3d5");
    Position p8("2a6");

    Position p9("6b6");
    Position p10("1d6");

    assert(p5 == p6);
    assert(p7 == p8);
    assert(p9 == p10);

    // Edge cases
    Position p11("OB");
    Position p12("V");

    assert(p11 == p11);
    assert(p12 == p12);
    assert(p11 != p12);
    assert(p5 != p11);
    assert(p6 != p12);
}

void TestPosition::test_sanitizedConstructor() {
    Position p1 (1, 'a', 6);
    assert(p1.getFlap() == 2);
    assert(p1.getRow() == 'd');
    assert(p1.getColumn() == 5);

    Position p2 ("2b5");
    assert(p2.getFlap() == 3);
    assert(p2.getRow() == 'e');
    assert(p2.getColumn() == 6);
}