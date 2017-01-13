#ifndef DUCHESS_CPP_LEGACYUTILS_H
#define DUCHESS_CPP_LEGACYUTILS_H

#include "../duchess/Board.h"
#include "../duchess/Position.h"
#include "../duchess/Piece.h"

#include <string>

class LegacyUtils {
public:
    static int legacyPieceNumberToPieceIndex(const int pieceNumber);
    static int pieceIndexToLegacyPieceNumber(const short owner, const int index);
    static int legacyPieceNumberToOwner(const int pieceNumber);
    static Position* legacySquareToPosition(const int square);
    static int positionToLegacyPosIndex(const Position* pos);
    static int pieceToModernRelativeIndex(const Piece* t_piece, const Board* t_board);
    static std::string moveToMoveData(const Move& m, const Piece* activePiece, const bool gameIsOver, const Board* board);
    static short dataToPlayerTurn(const char incomingData[]);

};

#endif //DUCHESS_CPP_LEGACYUTILS_H
