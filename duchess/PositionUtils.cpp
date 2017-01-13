#define NDEBUG

#include <iostream>
#include <cstdlib>
#include <vector>
#include <assert.h>
#include <algorithm>

#include "PositionUtils.h"
#include "GeneralUtils.h"


// Note that this function exists for legacy reasons, and now just returns the board index tracked by the position itself
BoardIndex PositionUtils::positionToBoardIndexSixPlayer(const Position* t_position) {
    return t_position->getBoardIndex();
}

BoardIndex PositionUtils::positionToBoardIndexSixPlayerSlow(const Position* t_position) {
    BoardIndex index;
    if (t_position->isOffBoard()) {
        index = 0;
    } else if (t_position->isVortex()) {
        index = 157;
    } else {
        short flap = t_position->getFlap();
        short row = t_position->getRow() - 'a' + 1;
        short column = t_position->getColumn();

        assert(flap <= 6 && flap >= 1);
        assert(row <= 5 && row >= 1);
        assert(column <= 6 && column >= 1);

        if (column <= 4 && column >= 1) {
            index = ((flap - 1) * 5 * 4) + ((column - 1) * 5) + row;
        } else if (column <= 6) {
            short modifiedFlap = flap;
            short modifiedRow = row;
            short modifiedCol = column;
            if (row <= 2) {
                modifiedFlap += 1;
                if (modifiedFlap > 6) {
                    modifiedFlap -= 6;
                }
                modifiedRow = 6 - (column - 4);
                modifiedCol = row + 4;
            }

            index = 120 + ((modifiedFlap - 1) * 3 * 2) + ((modifiedCol - 1 - 4) * 3) + modifiedRow - 2;
        } else {
            // Throw exception
            assert(column >= 1 && column <= 6);
            index = 0;
        }
    }

    return index;
}

Position* PositionUtils::boardIndexToPositionSixPlayer(const BoardIndex t_boardIndex) {
    // Default values
    bool offBoard = false;
    bool vortex = false;
    short flap = 0;
    char row = '\0';
    short column = 0;

    if (t_boardIndex == 0) {
        // Off Board
        offBoard = true;
    } else if (t_boardIndex == 157) {
        // Vortex
        vortex = true;
    } else {
        // The position is defined by three sequential values in the lookup table
        const unsigned int lookupIndex = t_boardIndex * 3;
        flap = sixPlayerIndexToPosition[lookupIndex];
        row = sixPlayerIndexToPosition[lookupIndex + 1];
        column = sixPlayerIndexToPosition[lookupIndex + 2];
    }

    return new Position(flap, row, column, offBoard, vortex);
}


std::vector<BoardIndex> PositionUtils::getAdjacentSquareIndices(const Position* t_position) {
    assert(!t_position->isOffBoard());
    if (!t_position->isVortex()) {
        return getAdjacentSquareIndicesLookup(t_position);
//        return getAdjacentSquareIndicesSlow(t_position);
    } else {
        std::vector<BoardIndex> adjacents {124, 130, 136, 142, 148, 154};
        return adjacents;
    }
}

std::vector<BoardIndex> PositionUtils::getAdjacentSquareIndicesLookup(const Position* t_position) {
    assert(!t_position->isVortex());
    std::vector<BoardIndex> adjacents;
    const BoardIndex index = positionToBoardIndexSixPlayer(t_position) - 1;
    int i;
    for (i = DIRECTION_CLOCKWISE; i <= DIRECTION_OUT; ++i) {
        const BoardIndex boardIndex = tablePositionAndDirectionToPosition[index][i];
        if (boardIndex != 0) {
            adjacents.push_back(boardIndex);
        }
    }
    return adjacents;
}

std::vector<BoardIndex> PositionUtils::getAdjacentSquareIndicesSlow(const Position* t_position) {
    assert(!t_position->isOffBoard());

    std::vector<BoardIndex> adjacentSquares;
    if (t_position->isVortex()) {
        adjacentSquares = {124, 130, 136, 142, 148, 154};
    } else {

        // Is there a position in the column to the left?
        const BoardIndex outerAdjacentColumnIndex = getIndexOfOuterAdjacentColumn(t_position);
        if (outerAdjacentColumnIndex != 0) {
            adjacentSquares.push_back(outerAdjacentColumnIndex);
        }

        // Is there a position in the column to the right?
        const BoardIndex innerAdjacentColumnIndex = getIndexOfInnerAdjacentColumn(t_position);
        if (innerAdjacentColumnIndex != 0) {
            adjacentSquares.push_back(innerAdjacentColumnIndex);
        }

        // Is there a position in the row to the left?
        const BoardIndex clockwiseAdjacentRowIndex = getIndexOfClockwiseAdjacentSquare(t_position);
        if (clockwiseAdjacentRowIndex != 0) {
            adjacentSquares.push_back(clockwiseAdjacentRowIndex);
        }

        // Is there a position in the row to the right?
        const BoardIndex antiClockwiseAdjacentRowIndex = getIndexOfAnticlockwiseAdjacentSquare(t_position);
        if (antiClockwiseAdjacentRowIndex != 0) {
            adjacentSquares.push_back(antiClockwiseAdjacentRowIndex);
        }

    }

    return adjacentSquares;
}

std::vector<BoardIndex> PositionUtils::getDiagonalSquareIndices(const Position* t_position) {
    assert(!t_position->isOffBoard());
    if (!t_position->isVortex()) {
        return getDiagonalSquareIndicesLookup(t_position);
//        return getDiagonalSquareIndicesSlow(t_position);
    } else {
        std::vector<BoardIndex> adjacents {125, 131, 137, 143, 149, 155};
        return adjacents;
    }
}

std::vector<BoardIndex> PositionUtils::getDiagonalSquareIndicesLookup(const Position* t_position) {
    assert(!t_position->isVortex());
    std::vector<BoardIndex> adjacents;
    const BoardIndex index = positionToBoardIndexSixPlayer(t_position) - 1;
    int i;
    for (i = DIRECTION_IN_CLOCKWISE; i <= DIRECTION_OUT_ANTICLOCKWISE; ++i) {
        const BoardIndex boardIndex = tablePositionAndDirectionToPosition[index][i];
        if (boardIndex != 0) {
            adjacents.push_back(boardIndex);
        }
    }
    return adjacents;
}

std::vector<BoardIndex> PositionUtils::getDiagonalSquareIndicesSlow(const Position* t_position) {
    assert(!t_position->isOffBoard());

    std::vector<BoardIndex> diagonalSquares;
    if (t_position->isVortex()) {
        diagonalSquares = {125, 131, 137, 143, 149, 155};
    } else {
        const BoardIndex innerClockwise = getIndexOfInnerClockwiseSquare(t_position);
        const BoardIndex outerClockwise = getIndexOfOuterClockwiseSquare(t_position);
        const BoardIndex innerAnticlockwise = getIndexOfInnerAnticlockwiseSquare(t_position);
        const BoardIndex outerAnticlockwise = getIndexOfOuterAnticlockwiseSquare(t_position);

        if (innerClockwise != 0) {
            diagonalSquares.push_back(innerClockwise);
        }
        if (outerClockwise != 0) {
            diagonalSquares.push_back(outerClockwise);
        }
        if (innerAnticlockwise != 0) {
            diagonalSquares.push_back(innerAnticlockwise);
        }
        if (outerAnticlockwise != 0) {
            diagonalSquares.push_back(outerAnticlockwise);
        }

    }

    return diagonalSquares;
}

std::vector<BoardIndex> PositionUtils::getAdjacentAndDiagonalSquareIndices(const Position* t_position) {
    assert(!t_position->isOffBoard());
    if (!t_position->isVortex()) {
        return getAdjacentAndDiagonalSquareIndicesLookup(t_position);
//        return getAdjacentAndDiagonalSquareIndicesSlow(t_position);
    } else {
        std::vector<BoardIndex> adjacents {124, 130, 136, 142, 148, 154, 125, 131, 137, 143, 149, 155};
        return adjacents;
    }
}

std::vector<BoardIndex> PositionUtils::getAdjacentAndDiagonalSquareIndicesLookup(const Position* t_position) {
    assert(!t_position->isVortex());
    std::vector<BoardIndex> adjacents;
    const BoardIndex index = positionToBoardIndexSixPlayer(t_position) - 1;
    int i;
    for (i = DIRECTION_CLOCKWISE; i <= DIRECTION_OUT_ANTICLOCKWISE; ++i) {
        const BoardIndex boardIndex = tablePositionAndDirectionToPosition[index][i];
        if (boardIndex != 0) {
            adjacents.push_back(boardIndex);
        }
    }
    return adjacents;
}

std::vector<BoardIndex> PositionUtils::getAdjacentAndDiagonalSquareIndicesSlow(const Position* t_position) {
    const std::vector<BoardIndex> adjacent = PositionUtils::getAdjacentSquareIndices(t_position);
    const std::vector<BoardIndex> diagonal = PositionUtils::getDiagonalSquareIndices(t_position);

    return GeneralUtils::mergeArrays(adjacent, diagonal);
}

BoardIndex PositionUtils::getIndexOfInnerAdjacentColumn(const Position* t_position) {
    assert(!t_position->isOffBoard() && !t_position->isVortex());
    return getIndexOfInnerAdjacentColumnLookup(t_position);
//    return getIndexOfInnerAdjacentColumnSlow(t_position);
}

BoardIndex PositionUtils::getIndexOfInnerAdjacentColumnLookup(const Position* t_position) {
    return tablePositionAndDirectionToPosition[positionToBoardIndexSixPlayer(t_position) - 1][PositionUtils::DIRECTION_IN];
}

BoardIndex PositionUtils::getIndexOfInnerAdjacentColumnSlow(const Position* t_position) {
    const short flap = t_position->getFlap();
    const char row = t_position->getRow();
    const short rightColumn = t_position->getColumn() + 1;

    if (rightColumn <= 6) {
        const Position rightPosition (flap, row, rightColumn);
        return PositionUtils::positionToBoardIndexSixPlayer(&rightPosition);
    } else if (rightColumn == 7 && row == 'c') {
        // This is the vortex
        const Position rightPosition ("V");
        return PositionUtils::positionToBoardIndexSixPlayer(&rightPosition);
    } else {
        const char newRow = 'c';
        short newFlap;
        if (row == 'a' || row == 'b') {
            // Go up one flap
            newFlap = (flap == 6 ? 1 : flap + 1);
        } else {
            // Go down one flap
            newFlap = (flap == 1 ? 6 : flap - 1);
        }
        const short newCol = ((row == 'a' || row == 'e') ? 5 : 6);

        const Position rightPosition (newFlap, newRow, newCol);
        return PositionUtils::positionToBoardIndexSixPlayer(&rightPosition);
    }
}

BoardIndex PositionUtils::getIndexOfOuterAdjacentColumn(const Position* t_position) {
    assert(!t_position->isVortex() && !t_position->isOffBoard());
    return getIndexOfOuterAdjacentColumnLookup(t_position);
//    return getIndexOfOuterAdjacentColumnSlow(t_position);
}

BoardIndex PositionUtils::getIndexOfOuterAdjacentColumnLookup(const Position* t_position) {
    return tablePositionAndDirectionToPosition[positionToBoardIndexSixPlayer(t_position) - 1][PositionUtils::DIRECTION_OUT];
}

BoardIndex PositionUtils::getIndexOfOuterAdjacentColumnSlow(const Position* t_position) {
    const short leftColumn = t_position->getColumn() - 1;
    if (leftColumn >= 1) {
        const Position leftPosition (t_position->getFlap(), t_position->getRow(), leftColumn);
        return PositionUtils::positionToBoardIndexSixPlayer(&leftPosition);
    } else {
        return 0;
    }
}

BoardIndex PositionUtils::getIndexOfClockwiseAdjacentSquare(const Position* t_position) {
    assert(!t_position->isOffBoard() && !t_position->isVortex());
    return getIndexOfClockwiseAdjacentSquareLookup(t_position);
//    return getIndexOfClockwiseAdjacentSquareSlow(t_position);
}

BoardIndex PositionUtils::getIndexOfClockwiseAdjacentSquareLookup(const Position* t_position) {
    return tablePositionAndDirectionToPosition[positionToBoardIndexSixPlayer(t_position) - 1][PositionUtils::DIRECTION_CLOCKWISE];
}

BoardIndex PositionUtils::getIndexOfClockwiseAdjacentSquareSlow(const Position* t_position) {
    const short flap = t_position->getFlap();
    const short column = t_position->getColumn();

    const short leftRow = t_position->getRow() - 1;
    if (leftRow >= 'a') {
        // Doesn't move us out of bounds
        const Position leftPosition (flap, leftRow, column);
        return PositionUtils::positionToBoardIndexSixPlayer(&leftPosition);
    } else if (column >= 5) {
        // Moves us out of bounds, but this just puts us on a new flap
        const short newFlap = (flap == 6 ? 1 : flap + 1);
        const short newRow = (column == 5 ? 'e' : 'd');
        const short newColumn = 4;

        const Position leftPosition (newFlap, newRow, newColumn);
        return PositionUtils::positionToBoardIndexSixPlayer(&leftPosition);
    } else {
        return 0;
    }
}

BoardIndex PositionUtils::getIndexOfAnticlockwiseAdjacentSquare(const Position* t_position) {
    assert(!t_position->isOffBoard() && !t_position->isVortex());
    return getIndexOfAnticlockwiseAdjacentSquareLookup(t_position);
//    return getIndexOfAnticlockwiseAdjacentSquareSlow(t_position);
}

BoardIndex PositionUtils::getIndexOfAnticlockwiseAdjacentSquareLookup(const Position* t_position) {
    return tablePositionAndDirectionToPosition[positionToBoardIndexSixPlayer(t_position) - 1][PositionUtils::DIRECTION_ANTICLOCKWISE];
}

BoardIndex PositionUtils::getIndexOfAnticlockwiseAdjacentSquareSlow(const Position* t_position) {
    const short flap = t_position->getFlap();
    const short column = t_position->getColumn();

    const short rightRow = t_position->getRow() + 1;
    if (rightRow <= 'e') {
        // Doesn't move us out of bounds
        const Position rightPosition (flap, rightRow, column);
        return PositionUtils::positionToBoardIndexSixPlayer(&rightPosition);
    } else if (column >= 5) {
        // Moves us out of bounds, but this just puts us on a new flap
        const short newFlap = (flap == 1 ? 6 : flap - 1);
        const short newRow = (column == 5 ? 'a' : 'b');
        const short newColumn = 4;

        const Position rightPosition (newFlap, newRow, newColumn);
        return PositionUtils::positionToBoardIndexSixPlayer(&rightPosition);
    } else {
        return 0;
    }
}


BoardIndex PositionUtils::getIndexOfInnerClockwiseSquare(const Position* t_position) {
    assert(!t_position->isOffBoard() && !t_position->isVortex());
    return getIndexOfInnerClockwiseSquareLookup(t_position);
//    return getIndexOfInnerClockwiseSquareSlow(t_position);
}

BoardIndex PositionUtils::getIndexOfInnerClockwiseSquareLookup(const Position* t_position) {
    return tablePositionAndDirectionToPosition[positionToBoardIndexSixPlayer(t_position) - 1][PositionUtils::DIRECTION_IN_CLOCKWISE];
}

BoardIndex PositionUtils::getIndexOfInnerClockwiseSquareSlow(const Position* t_position) {
    BoardIndex result;
    const char row = t_position->getRow();
    const short col = t_position->getColumn();
    const short flap = t_position->getFlap();

    const Position* t_innerPosition = boardIndexToPositionSixPlayer(getIndexOfInnerAdjacentColumn(t_position));

    if (!t_innerPosition->isOffBoard()) {
        // Edge cases
        if (col == 4 && row <= 'b') {
            // "IN" crosses over flap boundary on left side
            result = getIndexOfOuterAdjacentColumn(t_innerPosition);
        } else if (col == 6 && (row >= 'd')) {
            // "IN" crosses over flap boundary on right side
            result = getIndexOfInnerAdjacentColumn(t_innerPosition);
        } else if (t_innerPosition->isVortex()) {
            // "IN" puts us on vortex
            const short newFlap = (flap == 6 ? 1 : flap + 1);
            const Position resultPosition (newFlap, row, col);
            result = positionToBoardIndexSixPlayer(&resultPosition);
        } else {
            result = getIndexOfClockwiseAdjacentSquare(t_innerPosition);
        }
    } else {
        result = 0;
    }

    delete t_innerPosition;

    return result;
}

BoardIndex PositionUtils::getIndexOfOuterClockwiseSquare(const Position* t_position) {
    assert(!t_position->isOffBoard() && !t_position->isVortex());
    return getIndexOfOuterClockwiseSquareLookup(t_position);
//    return getIndexOfOuterClockwiseSquareSlow(t_position);
}

BoardIndex PositionUtils::getIndexOfOuterClockwiseSquareLookup(const Position* t_position) {
    return tablePositionAndDirectionToPosition[positionToBoardIndexSixPlayer(t_position) - 1][PositionUtils::DIRECTION_OUT_CLOCKWISE];
}

BoardIndex PositionUtils::getIndexOfOuterClockwiseSquareSlow(const Position* t_position) {
    const Position* t_outerPosition = boardIndexToPositionSixPlayer(getIndexOfOuterAdjacentColumn(t_position));
    const Position* t_clockwisePosition = boardIndexToPositionSixPlayer(getIndexOfClockwiseAdjacentSquare(t_position));

    BoardIndex result;
    if (!t_outerPosition->isOffBoard()) {
        result = getIndexOfClockwiseAdjacentSquare(t_outerPosition);
    } else if (!t_clockwisePosition->isOffBoard()) {
        result = getIndexOfOuterAdjacentColumn(t_clockwisePosition);
    } else {
        result = 0;
    }

    delete t_outerPosition;
    delete t_clockwisePosition;

    return result;
}

BoardIndex PositionUtils::getIndexOfInnerAnticlockwiseSquare(const Position* t_position) {
    assert(!t_position->isOffBoard() && !t_position->isVortex());
    return getIndexOfInnerAnticlockwiseSquareLookup(t_position);
//    return getIndexOfInnerAnticlockwiseSquareSlow(t_position);
}

BoardIndex PositionUtils::getIndexOfInnerAnticlockwiseSquareLookup(const Position* t_position) {
    return tablePositionAndDirectionToPosition[positionToBoardIndexSixPlayer(t_position) - 1][PositionUtils::DIRECTION_IN_ANTICLOCKWISE];
}

BoardIndex PositionUtils::getIndexOfInnerAnticlockwiseSquareSlow(const Position* t_position) {
    BoardIndex result;
    const char row = t_position->getRow();
    const short col = t_position->getColumn();
    const short flap = t_position->getFlap();

    const Position* t_innerPosition = boardIndexToPositionSixPlayer(getIndexOfInnerAdjacentColumn(t_position));

    if (!t_innerPosition->isOffBoard()) {
        // Edge cases
        if (col == 4 && row <= 'b') {
            // "IN" crosses over flap boundary on left side
            result = getIndexOfInnerAdjacentColumn(t_innerPosition);
        } else if (col == 6 && (row >= 'd')) {
            // "IN" crosses over flap boundary on right side
            result = getIndexOfOuterAdjacentColumn(t_innerPosition);
        } else if (t_innerPosition->isVortex()) {
            // "IN" puts us on vortex
            const short newFlap = (flap == 1 ? 6 : flap - 1);
            const Position resultPosition (newFlap, row, col);
            result = positionToBoardIndexSixPlayer(&resultPosition);
        } else {
            result = getIndexOfAnticlockwiseAdjacentSquare(t_innerPosition);
        }
    } else {
        result = 0;
    }

    delete t_innerPosition;

    return result;
}

BoardIndex PositionUtils::getIndexOfOuterAnticlockwiseSquare(const Position* t_position) {
    assert(!t_position->isOffBoard() && !t_position->isVortex());
    return getIndexOfOuterAnticlockwiseSquareLookup(t_position);
//    return getIndexOfOuterAnticlockwiseSquareSlow(t_position);
}

BoardIndex PositionUtils::getIndexOfOuterAnticlockwiseSquareLookup(const Position* t_position) {
    return tablePositionAndDirectionToPosition[positionToBoardIndexSixPlayer(t_position) - 1][PositionUtils::DIRECTION_OUT_ANTICLOCKWISE];
}

BoardIndex PositionUtils::getIndexOfOuterAnticlockwiseSquareSlow(const Position* t_position) {
    const Position* t_outerPosition = boardIndexToPositionSixPlayer(getIndexOfOuterAdjacentColumn(t_position));
    const Position* t_anticlockwisePosition = boardIndexToPositionSixPlayer(getIndexOfAnticlockwiseAdjacentSquare(t_position));

    BoardIndex result;

    if (!t_outerPosition->isOffBoard()) {
        result = getIndexOfAnticlockwiseAdjacentSquare(t_outerPosition);
    } else if (!t_anticlockwisePosition->isOffBoard()) {
        result = getIndexOfOuterAdjacentColumn(t_anticlockwisePosition);
    } else {
        result = 0;
    }

    delete t_outerPosition;
    delete t_anticlockwisePosition;

    return result;
}

BoardIndex PositionUtils::getIndexOfNextSquareInLine(const Position *t_pos1, const Position *t_pos2,
                                                         const short t_directionUsed) {
    assert(!t_pos2->isOffBoard());

    BoardIndex result;
    if (t_pos2->isVortex()) {
        // If pos2 is the vortex, the next in line is the "opposite" of pos1
        const short origFlap = t_pos1->getFlap();
        const short newFlap = (origFlap == 3 ? 6 : (origFlap + 3) % 6);
        const Position nextPos (newFlap, t_pos1->getRow(), t_pos1->getColumn());
        result = PositionUtils::positionToBoardIndexSixPlayer(&nextPos);

    } else {
        // If moving between po1 and 2 put us over a flap boundary, we need to apply some logic to finding
        // the direction of the next square
        const short flapChange = PositionUtils::getFlapChange(t_pos1, t_pos2);
        const short directionOfNextSquare = determineNextDirectionGivenPreviousDirectionAndFlapChange(t_directionUsed, flapChange);

        // Now that we know the relative direction of the next square, just use the appropriate helper method
        result = getIndexOfPositionInDirection(t_pos2, directionOfNextSquare);
    }

    return result;
}

BoardIndex PositionUtils::getIndexOfPositionInDirection(const Position* t_position, const short t_direction) {
    assert(!t_position->isVortex() && !t_position->isOffBoard());
    return getIndexOfPositionInDirectionLookup(t_position, t_direction);
//    return getIndexOfPositionInDirectionSlow(t_position, t_direction);
}

BoardIndex PositionUtils::getIndexOfPositionInDirectionLookup(const Position* t_position, const short t_direction) {
    return tablePositionAndDirectionToPosition[positionToBoardIndexSixPlayer(t_position) - 1][t_direction];
}

BoardIndex PositionUtils::getIndexOfPositionInDirectionSlow(const Position* t_position, const short t_direction) {
    BoardIndex result;

    if (t_direction == DIRECTION_IN) {
        result = getIndexOfInnerAdjacentColumn(t_position);
    } else if (t_direction == DIRECTION_OUT) {
        result = getIndexOfOuterAdjacentColumn(t_position);
    } else if (t_direction == DIRECTION_CLOCKWISE) {
        result = getIndexOfClockwiseAdjacentSquare(t_position);
    } else if (t_direction == DIRECTION_ANTICLOCKWISE) {
        result = getIndexOfAnticlockwiseAdjacentSquare(t_position);
    } else if (t_direction == DIRECTION_IN_CLOCKWISE) {
        result = getIndexOfInnerClockwiseSquare(t_position);
    } else if (t_direction == DIRECTION_IN_ANTICLOCKWISE) {
        result = getIndexOfInnerAnticlockwiseSquare(t_position);
    } else if (t_direction == DIRECTION_OUT_CLOCKWISE) {
        result = getIndexOfOuterClockwiseSquare(t_position);
    } else if (t_direction == DIRECTION_OUT_ANTICLOCKWISE) {
        result = getIndexOfOuterAnticlockwiseSquare(t_position);
    } else {
        result = 0;
    }

    return result;
}

short PositionUtils::getFlapChange(const Position* t_prev, const Position* t_cur) {
    if (t_cur->isVortex()) {
        return 3;
    } else if (t_prev->isVortex()) {
        return 0;
    } else {
        return (t_cur->getFlap() - t_prev->getFlap() + 12) % 6;
    }
}

short PositionUtils::determineNextDirectionGivenPreviousDirectionAndFlapChange(const short t_previousDirection, const short t_flapChange) {
    short directionOfNextSquare = t_previousDirection;
    if (t_flapChange == 5) {
        // FLAP-- (anticlockwise)
        if (t_previousDirection == DIRECTION_IN_ANTICLOCKWISE) {
            directionOfNextSquare = DIRECTION_OUT_ANTICLOCKWISE;
        } else if (t_previousDirection == DIRECTION_OUT_ANTICLOCKWISE) {
            directionOfNextSquare = DIRECTION_OUT_CLOCKWISE;
        } else if (t_previousDirection == DIRECTION_IN_CLOCKWISE) {
            directionOfNextSquare = DIRECTION_IN_ANTICLOCKWISE;
        } else if (t_previousDirection == DIRECTION_IN) {
            directionOfNextSquare = DIRECTION_ANTICLOCKWISE;
        } else if (t_previousDirection == DIRECTION_ANTICLOCKWISE) {
            directionOfNextSquare = DIRECTION_OUT;
        }
    } else if (t_flapChange == 1) {
        // FLAP++ (clockwise)
        if (t_previousDirection == DIRECTION_IN_CLOCKWISE) {
            directionOfNextSquare = DIRECTION_OUT_CLOCKWISE;
        } else if (t_previousDirection == DIRECTION_IN_ANTICLOCKWISE) {
            directionOfNextSquare = DIRECTION_IN_CLOCKWISE;
        } else if (t_previousDirection == DIRECTION_OUT_CLOCKWISE) {
            directionOfNextSquare = DIRECTION_OUT_ANTICLOCKWISE;
        } else if (t_previousDirection == DIRECTION_IN) {
            directionOfNextSquare = DIRECTION_CLOCKWISE;
        } else if (t_previousDirection == DIRECTION_CLOCKWISE) {
            directionOfNextSquare = DIRECTION_OUT;
        }
    } else if (t_flapChange == 3) {
        // Special case for traversal across vortex
        if (t_previousDirection == DIRECTION_IN) {
            directionOfNextSquare = DIRECTION_OUT;
        } else if (t_previousDirection == DIRECTION_OUT) {
            directionOfNextSquare = DIRECTION_IN;
        } else if (t_previousDirection == DIRECTION_IN_ANTICLOCKWISE) {
            directionOfNextSquare = DIRECTION_OUT_CLOCKWISE;
        } else if (t_previousDirection == DIRECTION_IN_CLOCKWISE) {
            directionOfNextSquare = DIRECTION_OUT_ANTICLOCKWISE;
        }
    }

    return directionOfNextSquare;
}

std::vector<BoardIndex> PositionUtils::getKnightMovesFromPosition(const Position* t_position) {
    assert(!t_position->isOffBoard());
    return getKnightMovesFromPositionLookup(t_position);
//    return getKnightMovesFromPositionSlow(t_position);
}


std::vector<BoardIndex> PositionUtils::getKnightMovesFromPositionLookup(const Position* t_position) {
    return PositionUtils::tablePositionToKnightMoves[positionToBoardIndexSixPlayer(t_position) - 1];
}

std::vector<BoardIndex> PositionUtils::getKnightMovesFromPositionSlow(const Position* t_position) {
    std::vector<BoardIndex> moves;
    if (t_position->isVortex()) {
        // Edge case for vortex
        moves = {126, 122, 132, 128, 138, 134, 144, 140, 150, 146, 156, 152};

    } else {

        std::vector<BoardIndex> diagonals = getDiagonalSquareIndices(t_position);
        std::vector<BoardIndex> adjacents = getAdjacentSquareIndices(t_position);

        // For each adjacent square, get the squares diagonal to it. If they're not in adjacents, add them to moves
        for (std::vector<BoardIndex>::iterator it = adjacents.begin(); it != adjacents.end(); ++it) {
            if (*it != 0) {
                const Position* p = boardIndexToPositionSixPlayer(*it);
                std::vector<BoardIndex> diagonalFromAdjacent = getDiagonalSquareIndices(p);
                for (std::vector<BoardIndex>::iterator dfaIt = diagonalFromAdjacent.begin(); dfaIt != diagonalFromAdjacent.end(); ++dfaIt) {
                    // If this square is not adjacent to t_position and it's not in moves, add it
                    const BoardIndex boardIndex = *dfaIt;
                    if (boardIndex != 0 &&
                            std::find(adjacents.begin(), adjacents.end(), boardIndex) == adjacents.end() &&
                            std::find(moves.begin(), moves.end(), boardIndex) == moves.end()) {
                        moves.push_back(boardIndex);
                    }
                }

                delete p;
            }
        }

        // For each diagonal square, get the squares adjacent to it. If they're not in adjacents, add them to moves
        for (std::vector<BoardIndex>::iterator it = diagonals.begin(); it != diagonals.end(); ++it) {
            if (*it != 0) {
                const Position* p = boardIndexToPositionSixPlayer(*it);
                std::vector<BoardIndex> adjacentToDiagonal = getAdjacentSquareIndices(p);
                for (std::vector<BoardIndex>::iterator atdIt = adjacentToDiagonal.begin(); atdIt != adjacentToDiagonal.end(); ++atdIt) {
                    // If this square is not adjacent to t_position and it's not in moves, add it
                    const BoardIndex boardIndex = *atdIt;
                    if (boardIndex != 0 &&
                            std::find(adjacents.begin(), adjacents.end(), boardIndex) == adjacents.end() &&
                            std::find(moves.begin(), moves.end(), boardIndex) == moves.end()) {
                        moves.push_back(boardIndex);
                    }
                }

                delete p;
            }

        }

        // Edge case: if t_pos is column 5 or 6, row c, need to add in c5/6 on flaps +- 2 from t_pos
        if (t_position->getColumn() >= 5 && t_position->getRow() == 'c') {
            const short flap = t_position->getFlap();
            const short p1Flap = (flap == 2 ? 6 : (flap - 2 + 6) % 6);
            const short p2Flap = (flap == 4 ? 6 : (flap + 2) % 6);
            const char targetRow = 'c';
            const short targetColumn = (t_position->getColumn() == 5 ? 6 : 5);
            const Position p1 (p1Flap, targetRow, targetColumn);
            const Position p2 (p2Flap, targetRow, targetColumn);

            moves.push_back(positionToBoardIndexSixPlayer(&p1));
            moves.push_back(positionToBoardIndexSixPlayer(&p2));
        }
    }

    return moves;
}

bool PositionUtils::canPieceMoveInDirection(const PieceType t_pieceType, const short t_direction) {
    if (t_direction >= DIRECTION_CLOCKWISE && t_direction <= DIRECTION_OUT) {
        return t_pieceType == PieceType::ROOK || t_pieceType == PieceType::FORTRESS || t_pieceType == PieceType::QUEEN;
    } else if (t_direction >= DIRECTION_IN_CLOCKWISE && t_direction <= DIRECTION_OUT_ANTICLOCKWISE) {
        return t_pieceType == PieceType::BISHOP || t_pieceType == PieceType::QUEEN || t_pieceType == PieceType::DUCHESS;
    } else {
        return false;
    }
}

int PositionUtils::distanceToVortex(const Position* t_position) {
    assert(!t_position->isOffBoard());
    if (t_position->isVortex()) {
        return 0;
    } else {
        const char row = t_position->getRow();
        const int distRow = row > 'c' ? row - 'c' : 'c' - row;
        const int distCol = 7 - t_position->getColumn();

        return distRow + distCol;
    }
}

short PositionUtils::stepsToPosition(const short t_pieceTypeIndex, const Position* t_startPosition, const Position* t_endPosition) {
    assert(!t_startPosition->isOffBoard() && !t_endPosition->isOffBoard());
    assert(t_pieceTypeIndex < 8);
    return tablePositionStepsToPosition[t_pieceTypeIndex][t_startPosition->getBoardIndex() - 1][t_endPosition->getBoardIndex() - 1];
}
