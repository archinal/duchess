// hash define NDEBUG if you want to turn off asserts

#include "../game/Game.h"
#include "../game/GameLog.h"
#include "../game/GameSetLog.h"
#include "../player/Player.h"
#include "../player/PlayerType.h"
#include "../player/Heuristic.h"
#include "../player/FeatureSet.h"
#include "../duchess/EvaluationMode.h"

#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <ctime>
#include <map>

/*
 * This program runs games of Duchess, training the
 */

static std::string runGame(GameSetLog& setLog, const EvaluationMode evaluationMode, const Heuristic heuristic, const FeatureSet featureSet, const std::string weightsPath, const std::string resultPath);

static std::string getTimeStamp();
static std::string getExperimentSetName();

int main(const int argc, const char* argv[]) {


    const std::vector<FeatureSet> featureSets {
            FeatureSet::MATERIAL,
            FeatureSet::MATERIAL_NAIVEATTDEF,
            FeatureSet::MATERIAL_ATTDEF,
            FeatureSet::MATERIAL_NAIVEATTDEF_PIECESQUARE,
            FeatureSet::MATERIAL_ATTDEF_PIECESQUARE,
            FeatureSet::MATERIAL_ATTDEF_KINGDIST,
            FeatureSet::MATERIAL_ATTDEF_KINGDIST_PIECE_SQUARE
    };

    const int numTrainingRounds = featureSets.size();
    int roundNumber;

    std::map<FeatureSet, std::string> baseWeightsPath;
    baseWeightsPath[FeatureSet::MATERIAL] = "../weights/wcci/material2.wts";
    baseWeightsPath[FeatureSet::MATERIAL_NAIVEATTDEF] = "../weights/training/results/material2.wts";
    baseWeightsPath[FeatureSet::MATERIAL_ATTDEF] = "../weights/training/results/material2.wts";
    baseWeightsPath[FeatureSet::MATERIAL_NAIVEATTDEF_PIECESQUARE] = "../weights/training/results/materialNaiveAttDef2.wts";
    baseWeightsPath[FeatureSet::MATERIAL_ATTDEF_PIECESQUARE] = "../weights/training/results/materialAttDef2.wts";
    baseWeightsPath[FeatureSet::MATERIAL_ATTDEF_KINGDIST] = "../weights/stage1.wts";
    baseWeightsPath[FeatureSet::MATERIAL_ATTDEF_KINGDIST_PIECE_SQUARE] = "../weights/training/results/stage2c.wts";

    std::map<FeatureSet, std::string> resultWeightsPath;
    resultWeightsPath[FeatureSet::MATERIAL] = "../weights/training/results/material2.wts";
    resultWeightsPath[FeatureSet::MATERIAL_NAIVEATTDEF] = "../weights/training/results/materialNaiveAttDef2.wts";
    resultWeightsPath[FeatureSet::MATERIAL_ATTDEF] = "../weights/training/results/materialAttDef2.wts";
    resultWeightsPath[FeatureSet::MATERIAL_NAIVEATTDEF_PIECESQUARE] = "../weights/training/results/materialNaiveADPS2.wts";
    resultWeightsPath[FeatureSet::MATERIAL_ATTDEF_PIECESQUARE] = "../weights/training/results/materialADPS2.wts";
    resultWeightsPath[FeatureSet::MATERIAL_ATTDEF_KINGDIST] = "../weights/training/results/stage2c.wts";
    resultWeightsPath[FeatureSet::MATERIAL_ATTDEF_KINGDIST_PIECE_SQUARE] = "../weights/training/results/stage3c.wts";

    for (roundNumber = 1; roundNumber <= numTrainingRounds; ++roundNumber) {

        std::stringstream gameSetLogDest;
        gameSetLogDest << "../logs/gameSets/";
        gameSetLogDest << getExperimentSetName() << ".csv";
        GameSetLog setLog(gameSetLogDest.str());

        const std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
        const int trainingTimeMinutes = 12 * 60; // 12 hours
        std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
        int durationSoFar = 0;
        int gameNumber = 1;

        const EvaluationMode evaluationMode = EvaluationMode::CUMULATIVE;
        const Heuristic heuristic = Heuristic::HISTORY;
        const FeatureSet featureSet = featureSets[roundNumber - 1];

        std::string weightsPath = baseWeightsPath[featureSet];
        std::string finalWeightsPath = resultWeightsPath[featureSet];

        bool hasFinished = false;

        std::cout << getTimeStamp() << ": Start round " << roundNumber << std::endl;
        std::cout << getTimeStamp() << ": Running training for " << trainingTimeMinutes << " minutes on weights at " <<
        weightsPath << std::endl;

        while (!hasFinished) {
            // In the final pass, tell it to save the weights in finalWeightsPath
            if (durationSoFar >= trainingTimeMinutes) {
                std::cout << getTimeStamp() << ": Running final set and saving results in " << finalWeightsPath <<
                std::endl;
                runGame(setLog, evaluationMode, heuristic, featureSet, weightsPath, finalWeightsPath);
                hasFinished = true;
            } else {
                // In normal passes, just make a new file for the weights and return me the name of that file
                if (gameNumber > 0 && gameNumber % 10 == 0) {
                    std::cout << getTimeStamp() << ": Running game number " << gameNumber << std::endl;
                }

                weightsPath = runGame(setLog, evaluationMode, heuristic, featureSet, weightsPath, "");
            }
            ++gameNumber;
            currentTime = std::chrono::high_resolution_clock::now();
            durationSoFar = std::chrono::duration_cast<std::chrono::minutes>(currentTime - startTime).count();
        }

        std::cout << "Finished running all " << gameNumber - 1 << " training games" << std::endl;

        setLog.saveLogs();
    }

    return EXIT_SUCCESS;
}

static std::string getTimeStamp() {
    std::time_t curTime = std::time(nullptr);
    std::string timeStamp = std::ctime(&curTime);
    timeStamp.erase(std::remove(timeStamp.begin(), timeStamp.end(), '\n'), timeStamp.end());
    return timeStamp;
}

static std::string getExperimentSetName() {
    std::string id = getTimeStamp();
    std::replace(id.begin(), id.end(), ' ', '_');
    std::replace(id.begin(), id.end(), ':', '-');
    return id;
}

static std::string runGame(GameSetLog& setLog, const EvaluationMode evaluationMode, const Heuristic heuristic, const FeatureSet featureSet, const std::string weightsPath, const std::string resultPath) {
    // Change the types here to choose who should be AI, who should be HUMAN
    Player p1 (PlayerType::AI, 1);
    Player p2 (PlayerType::AI, 2);
    Player p3 (PlayerType::AI, 3);
    Player p4 (PlayerType::AI, 4);
    Player p5 (PlayerType::AI, 5);
    Player p6 (PlayerType::AI, 6);

    // Setup the brain
    Brain brain;
    brain.setPathToWeights(weightsPath);
    brain.setHeuristic(heuristic);
    brain.setFeatureSet(featureSet);
    brain.setLearning(true);
    brain.loadWeightsAndFillVoid();

    p1.setBrain(&brain);
    p2.setBrain(&brain);
    p3.setBrain(&brain);
    p4.setBrain(&brain);
    p5.setBrain(&brain);
    p6.setBrain(&brain);

    // AI should make some random moves
    p1.setStochastic(true);
    p2.setStochastic(true);
    p3.setStochastic(true);
    p4.setStochastic(true);
    p5.setStochastic(true);
    p6.setStochastic(true);

    const std::vector<Player*> players {&p1, &p2, &p3, &p4, &p5, &p6};

    brain.setEvaluationMode(evaluationMode);
    Game game (players, evaluationMode);

    // Set up log destinations
    std::stringstream logDest;
    logDest << "../logs/searchAnalytics/";
    logDest << game.getID() << ".csv";
    brain.setLogDestination(logDest.str());

    std::stringstream gameLogDest;
    gameLogDest << "../logs/games/";
    gameLogDest << game.getID() << ".csv";
    game.setLogLocation(gameLogDest.str());

    // Set up weights save path
    std::string weightSavePath;
    if (resultPath == "") {
        std::stringstream weightsDest;
        weightsDest << "../weights/training/interim/";
        weightsDest << game.getID() << ".wts";
        weightSavePath = weightsDest.str();
    } else {
        weightSavePath = resultPath;
    }
    brain.setWeightsSavePath(weightSavePath);

    game.run();

    brain.saveWeights();

    brain.writeLogs();

    setLog.addLog(game.getLog(), featureSet, heuristic, evaluationMode);

    return weightSavePath;
}
