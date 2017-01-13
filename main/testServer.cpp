#include <iostream>
#include "../server/test/testLegacyUtils.h"

int main(const int argc, const char* argv[]) {
    TestLegacyUtils::runTests();

    std::cout << "Tests passed!" << std::endl;

    return EXIT_SUCCESS;
}
