#include <iostream>

#include "../game/test/testGameLog.h"
#include "../game/test/testGameSetLog.h"

int main(const int argc, const char* argv[]) {
    TestGameLog::runTests();
    TestGameSetLog::runTests();

    std::cout << "Tests passed!" << std::endl;

    return EXIT_SUCCESS;
}
