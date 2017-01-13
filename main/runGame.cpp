// hash define NDEBUG if you want to turn off asserts
#define NDEBUG

#include "../game/Game.h"
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

const bool IS_LEARNING = false;

/*
 * This program creates and hosts a local game of Duchess.
 *
 * The player types are configurable (HUMAN or AI), and are each queried in turn for a move, given a board state
 *
 * The program ends when the game is finished and the players have been notified about the result
 *
 * See ../game/Game.h and ../player/Player.h for more details
 */
int main(const int argc, const char* argv[]) {
    // Change the types here to choose who should be AI, who should be HUMAN
    Player p1 (PlayerType::AI, 1);
    Player p2 (PlayerType::AI, 2);
    Player p3 (PlayerType::AI, 3);
    Player p4 (PlayerType::AI, 4);
    Player p5 (PlayerType::AI, 5);
    Player p6 (PlayerType::AI, 6);

    // Setup the brain
    Brain brain ("../weights/wcci/materialCheck.wts");
    brain.setHeuristic(Heuristic::KILLER_MOVE);
    brain.setFeatureSet(FeatureSet::MATERIAL);
    brain.setLearning(IS_LEARNING);
    if (IS_LEARNING) {
        brain.setWeightsSavePath("../weights/training/material/satNight.wts");
    }
    p1.setBrain(&brain);
    p2.setBrain(&brain);
    p3.setBrain(&brain);
    p4.setBrain(&brain);
    p5.setBrain(&brain);
    p6.setBrain(&brain);

    const std::vector<Player*> players {&p1, &p2, &p3, &p4, &p5, &p6};

    const EvaluationMode evaluationMode = EvaluationMode::CUMULATIVE;

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
    if (IS_LEARNING) {
        brain.saveWeights();
    }
    return EXIT_SUCCESS;
}
