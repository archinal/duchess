#ifndef DUCHESS_CPP_TESTHEURISTICCACHE_H
#define DUCHESS_CPP_TESTHEURISTICCACHE_H

class TestHeuristicCache {
public:
    static void runTests();
private:
    static void test_history();
    static void test_bestReply();
    static void test_killer();
};

#endif //DUCHESS_CPP_TESTHEURISTICCACHE_H
