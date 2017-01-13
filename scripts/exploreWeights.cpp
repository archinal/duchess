#include <string>
#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <cassert>

#include "../player/Brain.h"
#include "../player/FeatureUtils.h"

static void printUsage();
static void printHelp();
static void pollInput(const Brain& t_brain);

static void countWeights(const Brain& t_brain);
static void printIndexes();
static void printRange(const Brain& t_brain, const unsigned int start, const unsigned int end);
static void printWeight(const Brain& t_brain, const unsigned int index);
static void printKingDistInfo();
static void printPieceIndexes();

int main(const int argc, const char* argv[]) {
    if (argc < 2) {
        printUsage();
        return EXIT_SUCCESS;
    }

    Brain brain (argv[1]);

    if (brain.getWeights().size() == 0) {
        std::cout << "The specified file did not yield weights" << std::endl;
        return EXIT_SUCCESS;
    }

    printHelp();
    pollInput(brain);

    return EXIT_SUCCESS;
}

static void printUsage() {
    std::cout << "Usage: " << std::endl;
    std::cout << "    [PATH_TO_WEIGHTS]  The weights to explore" << std::endl;
}

static void printHelp() {
    std::cout << "Commands: " << std::endl;
    std::cout << "    c         Counts the number of weights" << std::endl;
    std::cout << "    i         Prints a table of helpful indexes" << std::endl;
    std::cout << "    r         Prints all weights in the range [X, Y)" << std::endl;
    std::cout << "    w         Prints the weight at index N" << std::endl;
    std::cout << "    h         Display this message" << std::endl;
    std::cout << "    k         King Dist further info" << std::endl;
    std::cout << "    p         Show piece indexes" << std::endl;
    std::cout << "    x         Exit" << std::endl;

}

static void pollInput(const Brain& t_brain) {
    std::string input;
    while (input != "x") {
        std::cout << "> ";
        input = "";
        std::cin >> input;
        const char command = input[0];
        if (command == 'c') {
            countWeights(t_brain);
        } else if (command == 'h') {
            printHelp();
        } else if (command == 'k') {
            printKingDistInfo();
        } else if (command == 'p') {
            printPieceIndexes();
        } else if (command == 'i') {
            printIndexes();
        } else if (command == 'r') {
            unsigned int start;
            unsigned int end;
            std::cout << "Start: ";
            std::cin >> start;
            std::cout << "End: ";
            std::cin >> end;
            printRange(t_brain, start, end);
        } else if (command == 'w') {
            unsigned int weight;
            std::cout << "Index: ";
            std::cin >> weight;
            printWeight(t_brain, weight);
        } else if (command != 'x') {
            std::cout << "Invalid command" << std::endl;
            printHelp();
        }
    }
}

static void countWeights(const Brain& t_brain) {
    std::cout << t_brain.getWeights().size() << std::endl;
}

static void printIndexes() {
    std::cout << "INDEXES:" << std::endl << std::endl;
    std::cout << "Material Weights start        " << FeatureUtils::INDEX_MATERIAL_WEIGHTS_START << std::endl;
    std::cout << "Partner wizard weights start  " << FeatureUtils::INDEX_PARTNER_WIZARD_WEIGHTS_START << std::endl;
    std::cout << "Pawn distance start           " << FeatureUtils::INDEX_PAWN_DISTANCE_WEIGHTS_START << std::endl;
    std::cout << "Bias start                    " << FeatureUtils::INDEX_BIAS_WEIGHTS_START << std::endl;
    std::cout << "Check start                   " << FeatureUtils::INDEX_CHECK_WEIGHTS_START << std::endl;
    std::cout << "Checkmate start               " << FeatureUtils::INDEX_CHECKMATE_WEIGHTS_START << std::endl;
    std::cout << "Attacking start               " << FeatureUtils::INDEX_ATTACKING_WEIGHTS_START << std::endl;
    std::cout << "Defending start               " << FeatureUtils::INDEX_DEFENDING_WEIGHTS_START << std::endl;
    std::cout << "Naive Attacking start         " << FeatureUtils::INDEX_ATTACKING_NAIVE_WEIGHTS_START << std::endl;
    std::cout << "Naive Defending start         " << FeatureUtils::INDEX_DEFENDING_NAIVE_WEIGHTS_START << std::endl;
    std::cout << "King distance start           " << FeatureUtils::INDEX_KING_DIST_WEIGHTS_START << std::endl;
    std::cout << "Piece Square start            " << FeatureUtils::INDEX_PIECE_SQUARE_WEIGHTS_START << std::endl;
    std::cout << "Piece Square Naive start      " << FeatureUtils::INDEX_PIECE_SQUARE_AFTER_NAIVE_WEIGHTS_START << std::endl;
    std::cout << "Piece Square king dist start  " << FeatureUtils::INDEX_PIECE_SQUARE_AFTER_KING_DIST_START << std::endl;
    std::cout << "Total for PS                  " << FeatureUtils::TOTAL_WEIGHTS << std::endl;
    std::cout << "Total for NPS                 " << FeatureUtils::TOTAL_WEIGHTS_NAIVE << std::endl;
    std::cout << "Total for KingDist            " << FeatureUtils::TOTAL_WEIGHTS_NAIVE << std::endl;

}

static void printRange(const Brain& t_brain, const unsigned int start, const unsigned int end) {
    unsigned int i;
    std::stringstream output;
    for (i = start; i < end; ++i) {
        output << t_brain.getWeights()[i] << " ";
    }
    std::cout << output.str() << std::endl;
}

static void printWeight(const Brain& t_brain, const unsigned int index) {
    std::cout << t_brain.getWeights()[index] << std::endl;
}

static void printKingDistInfo() {
    std::cout << "King distance further info:" << std::endl;
    std::cout << "Max dist Pawn:        " << FeatureUtils::MAX_DIST_PAWN << std::endl;
    std::cout << "Max dist Bishop:      " << FeatureUtils::MAX_DIST_BISHOP << std::endl;
    std::cout << "Max dist Knight:      " << FeatureUtils::MAX_DIST_KNIGHT << std::endl;
    std::cout << "Max dist Queen:       " << FeatureUtils::MAX_DIST_QUEEN << std::endl;
    std::cout << "Max dist Rook:        " << FeatureUtils::MAX_DIST_ROOK << std::endl;
    std::cout << "Max dist Duchess:     " << FeatureUtils::MAX_DIST_DUCHESS << std::endl;
    std::cout << "Max dist Wizard/King: " << FeatureUtils::MAX_DIST_WIZARD << std::endl;
    std::cout << "Max dist Fortress:    " << FeatureUtils::MAX_DIST_FORTRESS << std::endl;
    std::cout << "Max dist per piece:   " << FeatureUtils::NUM_KING_DIST_ENTRIES_PER_PIECE << std::endl;
}

static void printPieceIndexes() {
    std::cout << "Piece indexes:" << std::endl;
    std::cout << "Pawn:      " << FeatureUtils::PAWN_INDEX << std::endl;
    std::cout << "Bishop:    " << FeatureUtils::BISHOP_INDEX << std::endl;
    std::cout << "Knight:    " << FeatureUtils::KNIGHT_INDEX << std::endl;
    std::cout << "Queen:     " << FeatureUtils::QUEEN_INDEX << std::endl;
    std::cout << "Rook:      " << FeatureUtils::ROOK_INDEX << std::endl;
    std::cout << "Duchess:   " << FeatureUtils::DUCHESS_INDEX << std::endl;
    std::cout << "Wizard:    " << FeatureUtils::WIZARD_INDEX << std::endl;
    std::cout << "Fortress:  " << FeatureUtils::FORTRESS_INDEX << std::endl;
    std::cout << "King:      " << FeatureUtils::KING_INDEX << std::endl;

}