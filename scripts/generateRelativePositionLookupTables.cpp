#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include <cassert>

#include "../duchess/Position.h"
#include "../duchess/PositionUtils.h"
#include "../duchess/Board.h"
#include "../duchess/Piece.h"
#include "../player/FeatureUtils.h"
/**
 * Prints lookup tables for positions
 * Options:
 *      --knight Prints knight-moves-from-position lookup table [position] --> [results]
 *      --directions Prints lookup table of [position][direction] --> result
 *
 */

static void printKnightLookupTable();
static void printDirectionLookupTable();
static void printPositionsRelativeToSeatLookupTable();
static void printMovesToPositionLookupTable();
static void printUsage();
static void print2DArray(const std::vector<std::vector<BoardIndex>>& vecs);

int main(const int argc, const char* argv[]) {
    if (argc >= 2 && strcmp("--knight", argv[1]) == 0) {
        printKnightLookupTable();
    } else if (argc >= 2 && strcmp("--directions", argv[1]) == 0) {
        printDirectionLookupTable();
    } else if (argc >= 2 && strcmp("--perplayer", argv[1]) == 0) {
        printPositionsRelativeToSeatLookupTable();
    } else if (argc >= 2 && strcmp("--nummoves", argv[1]) == 0) {
        printMovesToPositionLookupTable();
    } else {
        printUsage();
    }

    return EXIT_SUCCESS;
}

static void printKnightLookupTable() {
    std::vector<std::vector<BoardIndex>> vecs;
    BoardIndex posIndex;
    for (posIndex = 1; posIndex <= 156; ++posIndex) {
        Position* pos = PositionUtils::boardIndexToPositionSixPlayer(posIndex);
        vecs.push_back(PositionUtils::getKnightMovesFromPosition(pos));
        delete pos;
    }

    print2DArray(vecs);
}

static void printDirectionLookupTable() {
    std::vector<std::vector<BoardIndex>> vecs;
    BoardIndex posIndex;
    for (posIndex = 1; posIndex <= 156; ++posIndex) {
        std::vector<BoardIndex> directions;
        short direction;
        Position* pos = PositionUtils::boardIndexToPositionSixPlayer(posIndex);
        for (direction = PositionUtils::DIRECTION_CLOCKWISE; direction <= PositionUtils::DIRECTION_OUT_ANTICLOCKWISE; ++direction) {
            directions.push_back(PositionUtils::getIndexOfPositionInDirection(pos, direction));
        }
        delete pos;
        vecs.push_back(directions);
    }

    print2DArray(vecs);
}

static void printUsage() {
    std::cout << "Usage: " << std::endl;
    std::cout << "    --knight         Print knight lookup table" << std::endl;
    std::cout << "    --directions     Print directional lookup table" << std::endl;
    std::cout << "    --perplayer      Print list of BoardIndexes relative to seat" << std::endl;
    std::cout << "    --nummoves       Print lookup table for PieceType->StartPos->EndPos giving num moves needed" << std::endl;

}

static void print2DArray(const std::vector<std::vector<BoardIndex>>& vecs) {
    BoardIndex i, j;
    std::cout << "{";
    for (i = 0; i < vecs.size(); ++i) {
        std::cout << "{";
        for (j = 0; j < vecs[i].size(); ++j) {
            std::cout << (int)vecs[i][j];
            if (j != vecs[i].size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "}";
        if (i != vecs.size() - 1) {
            std::cout << ", " << std::endl;
        }
    }
    std::cout << "}";
}

static void print3DArray(const std::vector<std::vector<std::vector<short int>>>& vecs) {
    unsigned int i, j, k;
    std::cout << "{";
    for (i = 0; i < vecs.size(); ++i) {
        std::cout << "{";
        for (j = 0; j < vecs[i].size(); ++j) {
            std::cout << "{";
            for (k = 0; k < vecs[i][j].size(); ++k) {
                std::cout << vecs[i][j][k];
                if (k != vecs[i][j].size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << "}";
            if (j != vecs[i].size() - 1) {
                std::cout << ", " << std::endl;
            }
        }
        std::cout << "}";
        if (i != vecs.size() - 1) {
            std::cout << ", " << std::endl;
        }
    }
    std::cout << "}";

}

static void printPositionsRelativeToSeatLookupTable() {
    std::vector<std::vector<BoardIndex>> vecs;

    short seat;
    for (seat = 0; seat < 6; ++seat) {
        std::vector<BoardIndex> vec;
        // Flaps
        const BoardIndex flapOffset = 20;
        BoardIndex i;
        for (i = 1; i <= 120; ++i) {
            BoardIndex index = i + (seat * flapOffset);
            index = index > 120 ? index - 120 : index;
            vec.push_back(index);
        }

        // Insides
        const BoardIndex baseOffset = 120;
        const BoardIndex vortOffset = 6;
        for (i = 1; i <= 6 * vortOffset; ++i) {
            BoardIndex index = (seat * vortOffset) + i;
            index = index > 6 * vortOffset ? index - (6 * vortOffset) : index;
            vec.push_back(index + baseOffset);
        }

        // Vortex is same for everyone
        vec.push_back(157);

        vecs.push_back(vec);
    }

    print2DArray(vecs);
}

static void printMovesToPositionLookupTable() {
    // Initialise a board and take all pieces off it
    Board board;
    const BoardIndex offBoardIndex = 0;
    short player;
    for (player = 1; player <= 6; ++player) {
        for (auto const& piece : board.getPiecesForPlayer(player)) {
            board.setPiecePosition(piece, offBoardIndex);
        }
    }

    std::vector<std::vector<std::vector<short int>>> vecs;
    const BoardIndex numPositions = 157;
    int pieceIndex;
    for (pieceIndex = 0; pieceIndex < FeatureUtils::KING_INDEX; ++pieceIndex) {
        int maxDist = 0;
        short playerSetIndex = 0;
        Piece* pieceToUse = board.getPiecesForPlayer(1)[playerSetIndex];
        while (FeatureUtils::pieceTypeToIndex(pieceToUse->getType()) != pieceIndex) {
            ++playerSetIndex;
            pieceToUse = board.getPiecesForPlayer(1)[playerSetIndex];
        }
        assert(FeatureUtils::pieceTypeToIndex(pieceToUse->getType()) == pieceIndex);
//        std::cout << "Running for piece type " << pieceIndex << std::endl;
        std::vector<std::vector<short int>> distances;
        BoardIndex fromPos;
        for (fromPos = 1; fromPos <= numPositions; ++fromPos) {
//            std::cout << "Running from pos " << (int)fromPos << std::endl;
            board.setPiecePosition(pieceToUse, fromPos);
            // DistanceToPos tracks the number of steps needed to get from fromPos to [Index + 1] using the piece associated with PieceIndex
            // If the entry is -1 it means travel is impossible
            // Works dynamically, working out where it can get to in N steps, then for all places reachable in N steps, all accesible
            // squares from there are clearly accessible in at least N+1 steps
            // Iteration continues until there's an N for which we can reach no previously found squares
            std::vector<short int> distanceToPos;
            distanceToPos.reserve(numPositions);
            distanceToPos.assign(numPositions, -1);
            distanceToPos[fromPos - 1] = 0;
            short lastDistance = 0;
            bool distanceAdded = true;
            while (distanceAdded) {
                distanceAdded = false;
                unsigned int idx;
                for (idx = 0; idx < distanceToPos.size(); ++idx) {
                    if (distanceToPos[idx] == lastDistance) {
                        const BoardIndex bi = idx + 1;
                        board.setPiecePosition(pieceToUse, bi);
                        for (auto const& pos : board.getAccessibleSquares(pieceToUse)) {
                            if (distanceToPos[pos->getBoardIndex() - 1] == -1) {
                                distanceToPos[pos->getBoardIndex() - 1] = lastDistance + 1;
                                distanceAdded = true;
                            }
                        }
                    }
                }
                ++lastDistance;
            }
            if (lastDistance > maxDist) {
                maxDist = lastDistance;
            }
            distances.push_back(distanceToPos);
            board.setPiecePosition(pieceToUse, offBoardIndex);
        }
        vecs.push_back(distances);
//        std::cout << "max dist for piece index : " << pieceIndex << " is " << maxDist << std::endl; // 17
    }


    print3DArray(vecs);

}