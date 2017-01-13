

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
#include "../duchess/TeamType.h"

#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <ctime>

/*
 * This program creates and hosts a local game of Duchess.
 *
 * It pits two teams against each other. Each team shares a brain and uses the same feature set.
 *
 * The intention of this program is to benchmark two feature sets/weights sets against each other
 */

static TeamType runGame(const int t_gameNumber, GameSetLog& setLog, const FeatureSet features1, const std::string weights1, const FeatureSet features2, const std::string weights2);

static std::string getTimeStamp();
static std::string getExperimentSetName();

int main(const int argc, const char* argv[]) {
    std::stringstream gameSetLogDest;
    gameSetLogDest << "../logs/gameSets/";
    gameSetLogDest << getExperimentSetName() << ".csv";
    GameSetLog setLog(gameSetLogDest.str());

    const int totalGames = 100;

    int gameNumber;
    const int outputThreshold = std::max(totalGames / 10, 1);

    int t2wins = 0;
    int t1wins = 0;
    int draws = 0;

    const std::string weights1 = "../weights/stage2c.wts";
    const std::string weights2 = "../weights/stage3c.wts";
    const FeatureSet features1 = FeatureSet::MATERIAL_ATTDEF_KINGDIST;
    const FeatureSet features2 = FeatureSet::MATERIAL_ATTDEF_KINGDIST_PIECE_SQUARE;

    std::cout << getTimeStamp() << ": Starting " << totalGames << " competition games" << std::endl;
    std::cout << "Team 1 Weights: " << weights1 << std::endl;
    std::cout << "Team 2 Weights: " << weights2 << std::endl;

    for (gameNumber = 1; gameNumber <= totalGames; ++gameNumber) {
        const bool isOddGame = (gameNumber % 2 == 1);
        if (gameNumber % outputThreshold == 0) {
            std::cout << getTimeStamp() << ": Running game #" << gameNumber << std::endl;
        }

        const TeamType winner = runGame(gameNumber, setLog, features1, weights1, features2, weights2);

        if (winner == TeamType::EVENS) {
            if (isOddGame) {
                ++t2wins;
            } else {
                ++t1wins;
            }
        } else if (winner == TeamType::ODDS) {
            if (isOddGame) {
                ++t1wins;
            } else {
                ++t2wins;
            }
        } else if (winner == TeamType::NONE) {
            ++draws;
        }
        std::cout << "Wins by Team 1:  " << t1wins << std::endl;
        std::cout << "Wins by Team 2:  " << t2wins << std::endl;
        std::cout << "Draws:           " << draws << std::endl;
    }

    std::cout << getTimeStamp() << ": Finished running all " << totalGames << " competition games" << std::endl;
    std::cout << "Wins by Team 1:  " << t1wins << std::endl;
    std::cout << "Wins by Team 2:  " << t2wins << std::endl;
    std::cout << "Draws:           " << draws << std::endl;

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

static TeamType runGame(const int t_gameNumber, GameSetLog& setLog, const FeatureSet features1, const std::string weights1, const FeatureSet features2, const std::string weights2) {
    // Use this bool to help swap sides between games
    const bool isOddGame = t_gameNumber % 2 == 1;
    
    // Note that only brain1 performs logging (and GameLog only mentions the features1 as the feature set)
    const Heuristic heuristic = Heuristic::HISTORY;
    const EvaluationMode evaluationMode = EvaluationMode::CUMULATIVE;

    Player p1 (PlayerType::AI, 1);
    Player p2 (PlayerType::AI, 2);
    Player p3 (PlayerType::AI, 3);
    Player p4 (PlayerType::AI, 4);
    Player p5 (PlayerType::AI, 5);
    Player p6 (PlayerType::AI, 6);

    // AI should make some random moves
    p1.setStochastic(true);
    p2.setStochastic(true);
    p3.setStochastic(true);
    p4.setStochastic(true);
    p5.setStochastic(true);
    p6.setStochastic(true);

    // Setup the brain
    Brain brain1 (weights1);
    Brain brain2 (weights2);
    brain1.setHeuristic(heuristic);
    brain2.setHeuristic(heuristic);
    brain1.setFeatureSet(features1);
    brain2.setFeatureSet(features2);
    brain1.setLearning(false);
    brain2.setLearning(false);

    // Swap brains over between games
    p1.setBrain(isOddGame ? &brain1 : &brain2);
    p2.setBrain(isOddGame ? &brain2 : &brain1);
    p3.setBrain(isOddGame ? &brain1 : &brain2);
    p4.setBrain(isOddGame ? &brain2 : &brain1);
    p5.setBrain(isOddGame ? &brain1 : &brain2);
    p6.setBrain(isOddGame ? &brain2 : &brain1);

    const std::vector<Player*> players {&p1, &p2, &p3, &p4, &p5, &p6};

    brain1.setEvaluationMode(evaluationMode);
    brain2.setEvaluationMode(evaluationMode);
    Game game (players, evaluationMode);
//    std::stringstream logDest;
//    logDest << "../logs/searchAnalytics/";
//    logDest << game.getID() << ".csv";
//    brain1.setLogDestination(logDest.str());
//
//    std::stringstream gameLogDest;
//    gameLogDest << "../logs/games/";
//    gameLogDest << game.getID() << ".csv";
//    game.setLogLocation(gameLogDest.str());

    game.run();

    brain1.writeLogs();

    const TeamType winner = game.getLog().getWinner();

    setLog.addLog(game.getLog(), features1, heuristic, evaluationMode);

    return winner;
}
