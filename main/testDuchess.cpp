#include <iostream>
#include <cstdlib>

#include "../duchess/test/testPosition.h"
#include "../duchess/test/testPositionUtils.h"
#include "../duchess/test/testPiece.h"
#include "../duchess/test/testBoard.h"
#include "../duchess/test/testHolisticTests.h"
#include "../duchess/test/testMove.h"
#include "../duchess/test/testTeamUtils.h"
#include "../duchess/test/testMoveVector.h"
#include "../duchess/test/testMoveVectorUtils.h"

int main(const int argc, const char* argv[]) {
    TestPosition::runTests();
    TestPositionUtils::runTests();
    TestPiece::runTests();
    TestBoard::runTests();
    TestHolisticTests::runTests();
    TestMove::runTests();
    TestTeamUtils::runTests();
    TestMoveVector::runTests();
    TestMoveVectorUtils::runTests();

    std::cout << "Tests passed!" << std::endl;

    return EXIT_SUCCESS;
}