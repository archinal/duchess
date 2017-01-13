#include <stdio.h>
#include <iostream>
#include <cstdlib>

#include "../player/FeatureUtils.h"
#include "../server/LegacyUtils.h"
#include "../duchess/PositionUtils.h"

#define NUM_SQUARES 157

short xs6[NUM_SQUARES] = {
        -104, -51, 0, 51, 104, -99, -48, 0, 48, 99,
        -89, -45, 0, 45, 89, -83, -43, 0, 43, 83,
        -83, -43, 0, 43, -47, 0,
        -284, -264, -242, -213, -180, -254, -230, -208, -182, -155,
        -224, -200, -177, -155, -135, -195, -170, -146, -127, -112,
        -166, -134, -107, -91, -95, -62,
        -180, -213, -242, -264, -284, -155, -182, -208, -230, -254,
        -135, -155, -177, -200, -224, -112, -127, -146, -170, -195,
        -83, -91, -107, -134, -48, -62,
        104, 51, 0, -51, -104, 99, 48, 0, -48, -99,
        89, 45, 0, -45, -89, 83, 43, 0, -43, -83,
        83, 43, 0, -43, 47, 0,
        284, 264, 242, 213, 180, 254, 230, 208, 182, 155,
        224, 200, 177, 155, 135, 195, 170, 146, 127, 112,
        166, 134, 107, 91, 95, 62,
        180, 213, 242, 264, 284, 155, 182, 208, 230, 254,
        135, 155, 177, 200, 224, 112, 127, 146, 170, 195,
        83, 91, 107, 134, 48, 62, 0
        //       0,   0,   0,   0,   0,   0,   0,   0
};

short ys6[NUM_SQUARES] = {
        268, 275, 280, 275, 268, 236, 238, 240, 238, 236,
        207, 205, 204, 205, 207, 177, 171, 169, 171, 177,
        144, 130, 124, 130, 82, 72,
        44, 93, 140, 182, 224, 32, 77, 120, 161, 204,
        26, 64, 102, 141, 181, 17, 48, 85, 123, 160,
        0, 28, 62, 102, 0, 36,
        -224, -182, -140, -93, -44, -204, -161, -120, -77, -32,
        -181, -141, -102, -64, -26, -160, -123, -84, -48, -17,
        -144, -102, -62, -28, -82, -36,
        -268, -275, -280, -275, -268, -236, -238, -240, -238, -236,
        -207, -205, -204, -205, -207, -177, -171, -169, -171, -177,
        -144, -130, -124, -130, -82, -72,
        -44, -93, -140, -182, -224, -32, -77, -120, -161, -204,
        -26, -64, -102, -141, -181, -17, -48, -85, -123, -160,
        0, -28, -62, -102, 0, -36,
        224, 182, 140, 93, 44, 204, 161, 120, 77, 32,
        181, 141, 102, 64, 26, 160, 123, 85, 48, 17,
        144, 102, 62, 28, 82, 36, 0
        //     -46,   0,  46,  92, 138, 182, 226, 269
};

short xs[NUM_SQUARES];
short ys[NUM_SQUARES];

void plot_weights(double ws[NUM_SQUARES]) {
    int s;
    for (s = 0; s < NUM_SQUARES; s++) {
        // Uncomment to do for 4 squares
//        if (s == 156 || s % 78 < 52 || s % 78 >= 72) {
            if (ws[s] > 0.0) {
                printf("\\posCircle{%d,%d}{%1.4f};\n", xs[s], ys[s], 20000.0 * ws[s]);
            } else if (ws[s] < 0.0) {
                printf("\\negCircle{%d,%d}{%1.4f};\n", xs[s], ys[s], 20000.0 * ws[s]);
            }
//        }
    }
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        std::cout << "Usage: ./position.o [PIECE_INITIAL] [USE_KING_DIST] < WEIGHTS_FILE" << std::endl;
        std::cout << "If nothing is written for USE_KING_DIST, we assume it is not included in weights" << std::endl;
        return EXIT_FAILURE;
    }

    const bool useKingDist = argc >= 3;

    double ws[NUM_SQUARES];
    double x;
    int i, p, s;

    p = FeatureUtils::KING_INDEX;

    switch (argv[1][0]) {
        case 'K':
        case 'k':
            p = FeatureUtils::KING_INDEX;
            break;
        case 'W':
        case 'w':
            p = FeatureUtils::WIZARD_INDEX;
            break;
        case 'Q':
        case 'q':
            p = FeatureUtils::QUEEN_INDEX;
            break;
        case 'D':
        case 'd':
            p = FeatureUtils::DUCHESS_INDEX;
            break;
        case 'F':
        case 'f':
            p = FeatureUtils::FORTRESS_INDEX;
            break;
        case 'N':
        case 'n':
            p = FeatureUtils::KNIGHT_INDEX;
            break;
        case 'R':
        case 'r':
            p = FeatureUtils::ROOK_INDEX;
            break;
        case 'B':
        case 'b':
            p = FeatureUtils::BISHOP_INDEX;
            break;
        case 'P':
        case 'p':
            p = FeatureUtils::PAWN_INDEX;
            break;
    }

    for (s = 0; s < 156; s++) {
        xs[s] = xs6[(s + 26) % 156];
        ys[s] = -ys6[(s + 26) % 156];
    }
    xs[156] = xs6[156];
    ys[156] = -ys6[156];

    int startIndex = (useKingDist ? FeatureUtils::INDEX_PIECE_SQUARE_AFTER_KING_DIST_START : FeatureUtils::INDEX_PIECE_SQUARE_WEIGHTS_START) + (p * NUM_SQUARES);
    int numScanned = 0;

    // Skip over all weights until the one we need
    for (i = 0; i < startIndex; i++) {
        numScanned = scanf("%lf", &x);
    }

    // Scan the piece square weights for the piece p
    for (s = 0; s < NUM_SQUARES; s++) {
        // Convert the position index bi (from s) to a legacy position index idx
        const BoardIndex bi = (BoardIndex) s;
        Position* pos = PositionUtils::boardIndexToPositionSixPlayer(bi + 1);
        const unsigned int idx = LegacyUtils::positionToLegacyPosIndex(pos);
        delete pos;
        numScanned = scanf("%lf", &ws[idx]);
    }

    plot_weights(ws);

    if (numScanned == 0) {
        std::cout << "There may have been a problem skipping" << std::endl;
    }

    return EXIT_SUCCESS;
}

