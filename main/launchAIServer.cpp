#include <iostream>
#include <string>

#include "../server/Server.h"
#include "../player/Heuristic.h"
#include "../player/Brain.h"
#include "../player/Player.h"
#include "../player/PlayerType.h"
#include "../duchess/EvaluationMode.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: port_number" << std::endl;
        return EXIT_FAILURE;
    }
    Server server = Server(argv[1]);

    // Set up an AI player with a brain
    // Change this stuff if you want to change the weights set, the heuristic, the player type etc
    std::string pathToWeights = "../weights/stage3v2.wts";
    Brain brain;
    brain.setPathToWeights(pathToWeights);
    brain.setHeuristic(Heuristic::HISTORY);
    brain.setFeatureSet(FeatureSet::MATERIAL_ATTDEF_PIECESQUARE);
    brain.setEvaluationMode(EvaluationMode::CUMULATIVE);
    brain.loadWeightsAndFillVoid();

    Player player (PlayerType::AI, 1);
    player.setBrain(&brain);

    // Player should not make random moves
    player.setStochastic(false);

    server.setBrain(&brain);
    server.setPlayer(&player);

    server.run();

    return EXIT_SUCCESS;
}
