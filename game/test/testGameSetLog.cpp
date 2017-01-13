#include "testGameSetLog.h"

#include "../GameSetLog.h"
#include "../GameLog.h"
#include "../../player/Heuristic.h"
#include "../../player/FeatureSet.h"
#include "../../duchess/EvaluationMode.h"

#include <cassert>
#include <string>

void TestGameSetLog::runTests() {
    TestGameSetLog::test_all();
}

void TestGameSetLog::test_all() {
    GameSetLog gsl;
    GameLog g1;
    std::string dest = "destination";
    gsl.setLogDest(dest);
    assert(gsl.getLogDest() == dest);

    gsl.addLog(g1, FeatureSet::MATERIAL_ATTDEF, Heuristic::HISTORY, EvaluationMode::DE_NOVO);
}