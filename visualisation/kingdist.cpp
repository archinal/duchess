#include <iostream>
#include <vector>

#include "../player/Brain.h"
#include "../player/FeatureUtils.h"

static std::vector<double> scanWeights();
static void printWeights(const std::vector<double>& weights);
static void printSet(const std::vector<double>& weights, const int startOffset, const int maxDist);

int main(const int argc, const char* argv[]) {

    const std::vector<double>& weights = scanWeights();
    printWeights(weights);

    return EXIT_SUCCESS;
}

static std::vector<double> scanWeights() {
    // Burn off irrelevant weights from stdio
    const int offset = FeatureUtils::INDEX_KING_DIST_WEIGHTS_START;
    int i;
    double x;
    int numScanned = 0;
    for (i = 0; i < offset; ++i) {
        numScanned = scanf("%lf", &x);
    }

    std::vector<double> weights;
    weights.reserve(FeatureUtils::NUM_KING_DIST_WEIGHTS);

    for (i = 0; i < FeatureUtils::NUM_KING_DIST_WEIGHTS; ++i) {
        numScanned = scanf("%lf", &x);
        weights.push_back(x);
    }

    if (numScanned == 0) {
        std::cout << "There may have been a problem scanning" << std::endl;
    }

    return weights;
}

static void printSet(const std::vector<double>& weights, const int startOffset, const int maxDist) {
    int i;
    int king;
    for (king = 0; king < 3; ++king) {
        const int opponentOffset = king * FeatureUtils::NUM_KING_DIST_ENTRIES_PER_PIECE;
        std::cout << "Against opponent " << king + 1 << ":" << std::endl;
        for (i = 0; i < maxDist; ++i) {
            const double weight = weights[startOffset + i + opponentOffset];
            std::cout << weight;
            if (i != maxDist - 1) {
                std::cout << ", ";
            } else {
                std::cout << std::endl;
            }
        }
    }

    std::cout << std::endl;
}

static void printWeights(const std::vector<double>& weights) {
    int offset = 0;
    // Print pawn
    std::cout << "PAWN" << std::endl;
    printSet(weights, offset, FeatureUtils::MAX_DIST_PAWN);
    offset += FeatureUtils::MAX_DIST_PAWN;

    // Print bishop
    std::cout << "BISHOP" << std::endl;
    printSet(weights, offset, FeatureUtils::MAX_DIST_BISHOP);
    offset += FeatureUtils::MAX_DIST_BISHOP;

    // Print knight
    std::cout << "KNIGHT" << std::endl;
    printSet(weights, offset, FeatureUtils::MAX_DIST_KNIGHT);
    offset += FeatureUtils::MAX_DIST_KNIGHT;

    // Print queen
    std::cout << "QUEEN" << std::endl;
    printSet(weights, offset, FeatureUtils::MAX_DIST_QUEEN);
    offset += FeatureUtils::MAX_DIST_QUEEN;

    // Print rook
    std::cout << "ROOK" << std::endl;
    printSet(weights, offset, FeatureUtils::MAX_DIST_ROOK);
    offset += FeatureUtils::MAX_DIST_ROOK;

    // Print duchess
    std::cout << "DUCHESS" << std::endl;
    printSet(weights, offset, FeatureUtils::MAX_DIST_DUCHESS);
    offset += FeatureUtils::MAX_DIST_DUCHESS;

    // Print wizard
    std::cout << "WIZARD" << std::endl;
    printSet(weights, offset, FeatureUtils::MAX_DIST_WIZARD);
    offset += FeatureUtils::MAX_DIST_WIZARD;

    // Print fortress
    std::cout << "FORTRESS" << std::endl;
    printSet(weights, offset, FeatureUtils::MAX_DIST_FORTRESS);
    offset += FeatureUtils::MAX_DIST_FORTRESS;


}