#include <iostream>

#include "../player/test/testPlayer.h"
#include "../player/test/testBrain.h"
#include "../player/test/testSearchAnalytics.h"
#include "../player/test/testHeuristicCache.h"
#include "../player/test/testFeatureUtils.h"

int main(const int argc, const char* argv[]) {
    TestBrain::runTests();
    TestSearchAnalytics::runTests();
    TestHeuristicCache::runTests();
    TestFeatureUtils::runTests();
    TestPlayer::runTests();

    std::cout << "Tests passed!" << std::endl;

    return EXIT_SUCCESS;
}
