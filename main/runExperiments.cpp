
// hash define NDEBUG if you want to turn off asserts
#define NDEBUG

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
#include <algorithm>
#include <ctime>
#include <map>

/*
 * This program creates and hosts a local game of Duchess.
 *
 * The player types are configurable (HUMAN or AI), and are each queried in turn for a move, given a board state
 *
 * The program ends when the game is finished and the players have been notified about the result
 *
 * See ../game/Game.h and ../player/Player.h for more details
 */

static void runGame(GameSetLog& setLog, EvaluationMode evaluationMode, Heuristic heuristic, FeatureSet featureSet);

static std::string getTimeStamp();
static std::string getExperimentSetName();

int main(const int argc, const char* argv[]) {
    std::stringstream gameSetLogDest;
    gameSetLogDest << "../logs/gameSets/";
    gameSetLogDest << getExperimentSetName() << ".csv";
    GameSetLog setLog(gameSetLogDest.str());

    std::cout << getTimeStamp() << ": Starting experiments" << std::endl;

    const std::vector<EvaluationMode> evaluationModes {
            EvaluationMode::CUMULATIVE,
            EvaluationMode::DE_NOVO
    };
    const std::vector<Heuristic> heuristics {
            Heuristic::NONE,
            Heuristic::KILLER_MOVE,
            Heuristic::HISTORY,
            Heuristic::BEST_REPLY
    };
    const int totalExperiments = evaluationModes.size() * heuristics.size();

    int experimentNumber = 1;
    const int outputThreshold = std::max(totalExperiments / 10, 1);

    for (auto const& evaluationMode : evaluationModes) {
        for (auto const& heuristic : heuristics) {
            if (experimentNumber % outputThreshold == 0) {
                std::cout << getTimeStamp() << ": Running experiment #" << experimentNumber << std::endl;
            }

            runGame(setLog,
                    evaluationMode,
                    heuristic,
                    FeatureSet::MATERIAL);

            ++experimentNumber;
        }
    }



    std::cout << "Finished running all " << totalExperiments << " experiments" << std::endl;

    setLog.saveLogs();

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

static void runGame(GameSetLog& setLog, EvaluationMode evaluationMode, Heuristic heuristic, FeatureSet featureSet) {
    // Change the types here to choose who should be AI, who should be HUMAN
    Player p1 (PlayerType::AI, 1);
    Player p2 (PlayerType::AI, 2);
    Player p3 (PlayerType::AI, 3);
    Player p4 (PlayerType::AI, 4);
    Player p5 (PlayerType::AI, 5);
    Player p6 (PlayerType::AI, 6);

    // Setup the brain
    Brain brain ("../weights/training/results/material2.wts");
    brain.setHeuristic(heuristic);
    brain.setFeatureSet(featureSet);

    p1.setBrain(&brain);
    p2.setBrain(&brain);
    p3.setBrain(&brain);
    p4.setBrain(&brain);
    p5.setBrain(&brain);
    p6.setBrain(&brain);

    // Make sure no one moves randomly
    p1.setStochastic(false);
    p2.setStochastic(false);
    p3.setStochastic(false);
    p4.setStochastic(false);
    p5.setStochastic(false);
    p6.setStochastic(false);

    const std::vector<Player*> players {&p1, &p2, &p3, &p4, &p5, &p6};

    brain.setEvaluationMode(evaluationMode);
    Game game (players, evaluationMode);
    std::stringstream logDest;
    logDest << "../logs/searchAnalytics/";
    logDest << game.getID() << ".csv";
    brain.setLogDestination(logDest.str());

    std::stringstream gameLogDest;
    gameLogDest << "../logs/games/";
    gameLogDest << game.getID() << ".csv";
    game.setLogLocation(gameLogDest.str());

    game.run();

    brain.writeLogs();

    setLog.addLog(game.getLog(), featureSet, heuristic, evaluationMode);
}
