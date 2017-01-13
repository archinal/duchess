#include <stdio.h>
#include <iostream>
#include <cstdlib>

#include "../player/FeatureUtils.h"

double ac[6] = {0, 1, 2, 3, 4, 5};

void print_header(bool isAttacking) {
    int n;

    printf("\\documentclass{article}\n");
    printf("\\usepackage{tikz}\n\n");

    printf("\\newcommand{\\posWeight}[2]\n");
    printf("{\n");
    printf("  \\filldraw[draw=black,fill=blue] (#1) rectangle (#2);\n");
    printf("}\n\n");

    printf("\\newcommand{\\negWeight}[2]\n");
    printf("{\n");
    printf("  \\draw[draw=black] (#1) rectangle (#2);\n");
    printf("}\n\n");

    printf("\\begin{document}\n\n");

    printf("\\begin{tikzpicture}[scale=0.2]\n\n");

    // Draw the Hinton Grid
    printf("\\draw[black] (0,0) rectangle(30,60);\n\n");

    for (n = 10; n <= 30; n += 10) {
        printf("\\draw[black] (%d,0) -- (%d,60);\n", n, n);
    }
    for (n = 10; n <= 60; n += 10) {
        printf("\\draw[black] (0,%d) -- (30,%d);\n", n, n);
    }

    if (isAttacking) {
        printf("\\node[above] at (0,70){\\large Attacking Weights};\n");
        // Draw labels
        // Player labels on X
        for (n = 1; n <= FeatureUtils::NUM_PLAYERS / 2; ++n) {
            printf("\\node[above] at (%lf,63){O%d};\n", (n - 1.0) * 10.0 + 5, n);
        }
    } else {
        printf("\\node[above] at (0,70){\\large Defending Weights};\n");
        // Draw labels
        // Player labels on X
        for (n = 1; n <= FeatureUtils::NUM_PLAYERS / 2; ++n) {
            printf("\\node[above] at (%lf,63){T%d};\n", (n - 1.0) * 10.0 + 5, n);
        }
    }

    // Player labels on Y
    for (n = 1; n <= FeatureUtils::NUM_PLAYERS; ++n) {
        printf("\\node[left] at (-4.5,%lf){P%d};\n", 55.2 - ((n - 1.0) * 10), n);
    }

    // Piece labels on Y
    int p;
    for (n = 0; n < FeatureUtils::NUM_PLAYERS; ++n) {
        for (p = 0; p < FeatureUtils::NUM_UNIQUE_PIECES; ++p) {
            char label = '?';
            if (p == FeatureUtils::PAWN_INDEX) {
                label = 'P';
            } else if (p == FeatureUtils::BISHOP_INDEX) {
                label = 'B';
            } else if (p == FeatureUtils::KNIGHT_INDEX) {
                label = 'N';
            } else if (p == FeatureUtils::QUEEN_INDEX) {
                label = 'Q';
            } else if (p == FeatureUtils::ROOK_INDEX) {
                label = 'R';
            } else if (p == FeatureUtils::DUCHESS_INDEX) {
                label = 'D';
            } else if (p == FeatureUtils::WIZARD_INDEX) {
                label = 'W';
            } else if (p == FeatureUtils::FORTRESS_INDEX) {
                label = 'F';
            } else if (p == FeatureUtils::KING_INDEX) {
                label = 'K';
            }
            double x = p % 2 == 0 ? -3 : -1;
            double y = 58 - (n * 10) - p;
            printf("\\node[above] at (%lf,%lf){\\footnotesize %c};\n", x, y, label);
        }
    }

    // Piece labels on X
    for (n = 0; n < FeatureUtils::NUM_PLAYERS / 2; ++n) {
        for (p = 0; p < FeatureUtils::NUM_UNIQUE_PIECES; ++p) {
            char label = '?';
            if (p == FeatureUtils::PAWN_INDEX) {
                label = 'P';
            } else if (p == FeatureUtils::BISHOP_INDEX) {
                label = 'B';
            } else if (p == FeatureUtils::KNIGHT_INDEX) {
                label = 'N';
            } else if (p == FeatureUtils::QUEEN_INDEX) {
                label = 'Q';
            } else if (p == FeatureUtils::ROOK_INDEX) {
                label = 'R';
            } else if (p == FeatureUtils::DUCHESS_INDEX) {
                label = 'D';
            } else if (p == FeatureUtils::WIZARD_INDEX) {
                label = 'W';
            } else if (p == FeatureUtils::FORTRESS_INDEX) {
                label = 'F';
            } else if (p == FeatureUtils::KING_INDEX) {
                label = 'K';
            }
            double x = p + 1 + (n * 10);
            if (p == 3) {
                x += 0.1;
            }
            double y = 60;
            if (p % 2 == 0) {
                y += 1.4;
            } else if (p == 3) {
                y -= 0.3;
            }
            printf("\\node[above] at (%lf,%lf){\\footnotesize %c};\n", x, y, label);
        }
    }

}


int main(int argc, char* argv[]) {
    if (argc <= 1 || (argv[1][0] != 'A' && argv[1][0] != 'D')) {
        std::cout << "Usage: ./attack.o [A|D] < WEIGHTS" << std::endl;
        return EXIT_FAILURE;
    }

    const bool isAttacking = argv[1][0] == 'A';

    double w[6][9][6][9];
    double x, y, m;
    int i, a, b, c, d, p, q;
    int numScanned = 0;

    // Change to defending start if you want defending weights
    const int startIndex = isAttacking ? FeatureUtils::INDEX_ATTACKING_WEIGHTS_START : FeatureUtils::INDEX_DEFENDING_WEIGHTS_START;

    // Burn off unwanted weights
    for (i = 0; i < startIndex; i++) {
        numScanned = scanf("%lf", &x);
    }

    // Scan weights in
    // Active player
    for (a = 0; a < FeatureUtils::NUM_PLAYERS; a++) {
        // Active piece
        for (p = 0; p < FeatureUtils::NUM_UNIQUE_PIECES; p++) {
            // Passive player (1 of 3)
            for (b = 0; b < FeatureUtils::NUM_PLAYERS / 2; b++) {
                // Passive piece
                for (q = 0; q < FeatureUtils::NUM_UNIQUE_PIECES; q++) {
                    numScanned = scanf("%lf", &w[a][p][b][q]);
                }
            }
        }
    }

    print_header(isAttacking);

    // Player Active
    for (c = 0; c < FeatureUtils::NUM_PLAYERS; c++) {
        a = ac[c];
        // Active Piece
        for (p = 0; p < FeatureUtils::NUM_UNIQUE_PIECES; p++) {
            // Passive Player (only 3 players since only 3 can be attacked/defended
            for (d = 0; d < FeatureUtils::NUM_PLAYERS / 2; d++) {
                b = ac[d];
                // Passive Piece
                for (q = 0; q < FeatureUtils::NUM_UNIQUE_PIECES; q++) {
                    y = 59 - 10 * c - p;
                    x = 1 + 10 * d + q;
                    m = (isAttacking ? 250 : 150) * w[c][p][d][q];
                    if (m > 0) {
                        printf("\\posWeight{%1.4f,%1.4f}{%1.4f,%1.4f};\n",
                               x - m, y - m, x + m, y + m);
                    } else {
                        printf("\\negWeight{%1.4f,%1.4f}{%1.4f,%1.4f};\n",
                               x + m, y + m, x - m, y - m);
                    }
                    //printf(" %1.4f",w[a][b][p][q]);
                    //if( d < 3 || q < 8 ) {
                    //  printf(",");
                    //}
                }
            }
            printf("\n");
        }
    }
    printf("\\end{tikzpicture}\n\n");

    printf("\\end{document}\n");

    if (numScanned == 0) {
        std::cout << "Maybe an error scanning" << std::endl;
    }


    return EXIT_SUCCESS;
}
