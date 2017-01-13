#ifndef DUCHESS_POSITIONUTILS_H
#define DUCHESS_POSITIONUTILS_H

#include <vector>

#include "Position.h"
#include "PieceType.h"
// Forward declare position so we can use pointers to it
// class Position;

class PositionUtils {
public:
    static Position* boardIndexToPositionSixPlayer(const BoardIndex t_boardIndex);
    static BoardIndex positionToBoardIndexSixPlayer(const Position* t_position);
    static BoardIndex positionToBoardIndexSixPlayerSlow(const Position* t_position);

    // Given a position, return a board index relating to the positions adjacent (not diagonal) to that position
    static std::vector<BoardIndex> getAdjacentSquareIndices(const Position* t_position);
    static std::vector<BoardIndex> getAdjacentSquareIndicesLookup(const Position* t_position);
    static std::vector<BoardIndex> getAdjacentSquareIndicesSlow(const Position* t_position);

    // Given a position, return a board index relating to the positions adjacent (not diagonal) to that position
    static std::vector<BoardIndex> getDiagonalSquareIndices(const Position* t_position);
    static std::vector<BoardIndex> getDiagonalSquareIndicesLookup(const Position* t_position);
    static std::vector<BoardIndex> getDiagonalSquareIndicesSlow(const Position* t_position);

    static std::vector<BoardIndex> getAdjacentAndDiagonalSquareIndices(const Position* t_position);
    static std::vector<BoardIndex> getAdjacentAndDiagonalSquareIndicesLookup(const Position* t_position);
    static std::vector<BoardIndex> getAdjacentAndDiagonalSquareIndicesSlow(const Position* t_position);

    // Given a position, return a board index relating to the nearby knight moves from that position
    static std::vector<BoardIndex> getKnightMovesFromPosition(const Position* t_position);
    static std::vector<BoardIndex> getKnightMovesFromPositionSlow(const Position* t_position);
    static std::vector<BoardIndex> getKnightMovesFromPositionLookup(const Position* t_position);

    static BoardIndex getIndexOfInnerAdjacentColumn(const Position* t_position);
    static BoardIndex getIndexOfInnerAdjacentColumnSlow(const Position* t_position);
    static BoardIndex getIndexOfInnerAdjacentColumnLookup(const Position* t_position);
    static BoardIndex getIndexOfOuterAdjacentColumn(const Position* t_position);
    static BoardIndex getIndexOfOuterAdjacentColumnSlow(const Position* t_position);
    static BoardIndex getIndexOfOuterAdjacentColumnLookup(const Position* t_position);
    static BoardIndex getIndexOfClockwiseAdjacentSquare(const Position* t_position);
    static BoardIndex getIndexOfClockwiseAdjacentSquareSlow(const Position* t_position);
    static BoardIndex getIndexOfClockwiseAdjacentSquareLookup(const Position* t_position);
    static BoardIndex getIndexOfAnticlockwiseAdjacentSquare(const Position* t_position);
    static BoardIndex getIndexOfAnticlockwiseAdjacentSquareSlow(const Position* t_position);
    static BoardIndex getIndexOfAnticlockwiseAdjacentSquareLookup(const Position* t_position);

    static BoardIndex getIndexOfInnerClockwiseSquare(const Position* t_position);
    static BoardIndex getIndexOfInnerClockwiseSquareSlow(const Position* t_position);
    static BoardIndex getIndexOfInnerClockwiseSquareLookup(const Position* t_position);
    static BoardIndex getIndexOfOuterClockwiseSquare(const Position* t_position);
    static BoardIndex getIndexOfOuterClockwiseSquareSlow(const Position* t_position);
    static BoardIndex getIndexOfOuterClockwiseSquareLookup(const Position* t_position);
    static BoardIndex getIndexOfInnerAnticlockwiseSquare(const Position* t_position);
    static BoardIndex getIndexOfInnerAnticlockwiseSquareSlow(const Position* t_position);
    static BoardIndex getIndexOfInnerAnticlockwiseSquareLookup(const Position* t_position);
    static BoardIndex getIndexOfOuterAnticlockwiseSquare(const Position* t_position);
    static BoardIndex getIndexOfOuterAnticlockwiseSquareSlow(const Position* t_position);
    static BoardIndex getIndexOfOuterAnticlockwiseSquareLookup(const Position* t_position);

    static short getFlapChange(const Position* t_prev, const Position* t_cur);
    static short determineNextDirectionGivenPreviousDirectionAndFlapChange(const short t_previousDirection, const short t_flapChange);
    static BoardIndex getIndexOfPositionInDirection(const Position* t_position, const short t_direction);
    static BoardIndex getIndexOfPositionInDirectionSlow(const Position* t_position, const short t_direction);
    static BoardIndex getIndexOfPositionInDirectionLookup(const Position* t_position, const short t_direction);
    static BoardIndex getIndexOfNextSquareInLine(const Position* t_pos1, const Position* t_pos2, const short t_directionUsed);

    static bool canPieceMoveInDirection(const PieceType t_pieceType, const short t_direction);

    static int distanceToVortex(const Position* t_position);
    static short stepsToPosition(const short t_pieceTypeIndex, const Position* t_startPosition, const Position* t_endPosition);

    static const BoardIndex NUM_BOARD_INDICES_SIX_PLAYER = 158;
    static const BoardIndex NUM_VALID_POSITIONS_SIX_PLAYER = 182;

    static const short DIRECTION_CLOCKWISE = 0;
    static const short DIRECTION_ANTICLOCKWISE = 1;
    static const short DIRECTION_IN = 2;
    static const short DIRECTION_OUT = 3;
    static const short DIRECTION_IN_CLOCKWISE = 4;
    static const short DIRECTION_OUT_CLOCKWISE = 5;
    static const short DIRECTION_IN_ANTICLOCKWISE = 6;
    static const short DIRECTION_OUT_ANTICLOCKWISE = 7;

private:
    static const BoardIndex sixPlayerPositionToIndex[];
    static const char sixPlayerIndexToPosition[];
    static const BoardIndex tablePositionAndDirectionToPosition[][8];
    static const short tablePositionStepsToPosition[8][NUM_BOARD_INDICES_SIX_PLAYER - 1][NUM_BOARD_INDICES_SIX_PLAYER - 1]; // [NUM_PIECES - 1][NUM_POSITIONS][NUM_POSITIONS]
    static const std::vector<std::vector<BoardIndex>> tablePositionToKnightMoves;
};


#endif //DUCHESS_POSITIONUTILS_H
