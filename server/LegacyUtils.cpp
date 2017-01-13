#include "LegacyUtils.h"

#include <vector>
#include <sstream>
#include <cassert>
#include <iostream>
#include <string>
#include <cstring>

#include "../duchess/Board.h"
#include "../duchess/Position.h"
#include "../duchess/Piece.h"

int LegacyUtils::pieceIndexToLegacyPieceNumber(const short owner, const int index) {
    int num = (owner - 1) * 15;

    if (index == 12) {
        // King
        num += 0;
    } else if (index == 11) {
        // Wizard
        num += 1;
    } else if (index == 7) {
        // Queen
        num += 2;
    } else if (index == 9) {
        // Duchess
        num += 3;
    } else if (index == 13) {
        // Fortress
        num += 4;
    } else if (index == 6) {
        // Knight
        num += 5;
    } else if (index == 14) {
        // Rook 1
        num += 7;
    } else if (index == 10) {
        // Rook 2
        num += 6;
    } else if (index == 8) {
        // Bishop 1
        num += 9;
    } else if (index == 5) {
        // Bishop 2
        num += 8;
    } else {
        // Pawns
        num += index + 10;
    }

    return num;
}

int LegacyUtils::positionToLegacyPosIndex(const Position* pos) {
    if (pos->isVortex()) {
        return 156;
    } else if (pos->isOffBoard()) {
        return 157;
    } else {
        short legacyFlap = pos->getFlap();
        short legacyCol = pos->getColumn();
        char legacyRow = pos->getRow();
        // Note main difference from legacy to canonical flaps:
        // 1b5 is in the canonical, 1e6 is not
        // So for canonical --> legacy:
        // 1e5 --> 6a5
        // 1e6 --> 6b5
        // 1d6 --> 6b6
        // 1a6 --> 2d5
        if (legacyCol == 5 && legacyRow == 'e') {
            legacyRow = 'a';
            legacyCol = 5;
            legacyFlap = (legacyFlap == 1 ? 6 : legacyFlap - 1);
        } else if (legacyCol == 6 && legacyRow >= 'd') {
            legacyFlap = (legacyFlap == 1 ? 6 : legacyFlap - 1);
            if (legacyRow == 'e') {
                legacyCol = 5;
            }
            legacyRow = 'b';
        }

        // Flap offset
        int piecesPerFlap = 26;
        int index = (legacyFlap - 1) * piecesPerFlap;

        if (legacyCol <= 4) {
            index += (legacyCol - 1) * 5;
            index += legacyRow - 'a';
        } else {
            index += 20;
            if (legacyCol == 5) {
                index += legacyRow - 'a';
            } else {
                index += 4 + (legacyRow - 'b');
            }
        }

        return index;
    }
}


Position* LegacyUtils::legacySquareToPosition(const int square) {
    // Square 156 is the vortex
    if (square == 156) {
        return new Position("V");
    } else if (square == 157) {
        return new Position("OB");
    } else {
        int squareRelativeToFlap = square % 26;
        short flap = (square / 26) + 1; // 26 squares per flap
        short column;
        char row;

        if (squareRelativeToFlap < 20) {
            row = 'a' + (squareRelativeToFlap % 5);
            column = (squareRelativeToFlap / 5) + 1;
        } else if (squareRelativeToFlap < 24) {
            row = 'a' + squareRelativeToFlap - 20;
            column = 5;
        } else {
            row = 'b' + squareRelativeToFlap - 24;
            column = 6;
        }

        return new Position(flap, row, column);
    }
}

int LegacyUtils::legacyPieceNumberToOwner(const int pieceNumber) {
    return (pieceNumber / 15) + 1;
}

int LegacyUtils::legacyPieceNumberToPieceIndex(const int pieceNumber) {
    int legacyIndex = pieceNumber % 15;
    int index;
    if (legacyIndex == 0) {
        // King
        index = 12;
    } else if (legacyIndex == 1) {
        // Wizard
        index = 11;
    } else if (legacyIndex == 2) {
        // Queen
        index = 7;
    } else if (legacyIndex == 3) {
        // Duchess
        index = 9;
    } else if (legacyIndex == 4) {
        // Fortress
        index = 13;
    } else if (legacyIndex == 5) {
        // Knight
        index = 6;
    } else if (legacyIndex == 6) {
        // Rook 1
        index = 10;
    } else if (legacyIndex == 7) {
        // Rook 2
        index = 14;
    } else if (legacyIndex == 8) {
        // Bishop 1
        index = 5;
    } else if (legacyIndex == 9) {
        // Bishop 2
        index = 8;
    } else {
        // Pawns
        index = legacyIndex - 10;
    }

    return index;
}


std::string LegacyUtils::moveToMoveData(const Move &m, const Piece* activePiece, const bool gameIsOver, const Board* board) {
    std::cout << "Converting move to string: " << m.toString() << std::endl;

    int pawnReplace;
    const Piece* replacePiece = m.getReplacePiece();
    if (replacePiece == nullptr) {
        pawnReplace = -1;
    } else {
        pawnReplace = LegacyUtils::pieceIndexToLegacyPieceNumber(replacePiece->getOwner(), LegacyUtils::pieceToModernRelativeIndex(replacePiece, board));
    }
    const short activePieceOwner = activePiece->getOwner();
    const int activePieceLegacyIndex = LegacyUtils::pieceIndexToLegacyPieceNumber(activePieceOwner, LegacyUtils::pieceToModernRelativeIndex(activePiece, board));
    const int destinationSquareLegacyIndex = LegacyUtils::positionToLegacyPosIndex(m.getToPosition());
    const int ms = 0; // What's this?
    std::ostringstream os;
    os << pawnReplace << " " << activePieceLegacyIndex << " " << destinationSquareLegacyIndex << " " << ms << " " << (gameIsOver ? 1 : 0) << std::endl;

    std::cout << "Sending move data: " << os.str() << std::endl;

    return os.str();
}

short LegacyUtils::dataToPlayerTurn(const char incomingData[]) {
    int numSquares = 157;
    return incomingData[(numSquares * 2) + 2] + 1;
}

int LegacyUtils::pieceToModernRelativeIndex(const Piece* t_piece, const Board* t_board) {
    if (t_piece == nullptr) {
        return -1;
    } else {
        const short owner = t_piece->getOwner();
        int index;
        bool found = false;
        for (index = 0; index < 15 && !found; ++index) {
            if (t_board->getPiecesForPlayer(owner)[index] == t_piece) {
                found = true;
            }
        }
        assert(found);
        return index - 1;
    }
}