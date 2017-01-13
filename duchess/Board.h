#ifndef DUCHESS_BOARD_H
#define DUCHESS_BOARD_H

#include <vector>
#include <deque>
#include "Position.h"
#include "PositionUtils.h"
#include "Piece.h"
#include "PieceType.h"
#include "TeamType.h"
#include "Move.h"
#include "EvaluationMode.h"

class Board {
public:
    Board();
    Board(std::vector<std::vector<Piece>> t_pieces);

    ~Board();

    // Both of these may return nullptr (if there is no piece at that position)
    Piece* getPieceAtPosition(const Position* t_position) const;
    Piece* getPieceAtPosition(const BoardIndex t_positionIndex) const;

    // Updates our global understanding of where pieces are (m_squares) and the piece's understanding of where it is
    void setPiecePosition(Piece* t_piece, const BoardIndex t_positionIndex);
    void setPiecePosition(Piece* t_piece, const Position* t_position);

    Position* getPosition(const BoardIndex t_positionIndex) const;
    Position* getPosition(const Position* t_position) const;

    std::vector<Piece*> getPiecesForPlayer(const short t_playerNumber) const;

    std::vector<Move> getLegalMoves(const short t_player) const;
    std::vector<Move> getLegalMovesForPiece(const Piece* t_piece, bool t_kingInCheck, const std::vector<Position*>& t_blockableSquares) const;
    bool isLegalMove(const Move& t_move, const short t_player) const;

    // Given a piece, returns the positions it can move to (not necessarily check-safe though)
    std::vector<Position*> getAccessibleSquares(const Piece* t_piece) const;
    std::vector<Position*> getAccessibleSquaresAlongDirection(const Piece* t_piece, const short t_startDirection) const;
    std::vector<BoardIndex> getTeleportableSquares(const Piece* t_piece) const;
    bool doesSquareContainPiece(const Position* t_position) const;
    bool doesSquareContainEnemy(const Position* t_position, const Piece* t_piece) const;
    bool doesSquareContainAlly(const Position* t_position, const Piece* t_piece) const;
    bool isPieceWizardAdjacent(const Piece* t_piece) const;
    std::vector<Piece*> getAdjacentPieces(const Piece* t_piece) const;

    // Returns a list of positions that if entered will block all incoming attacks on the player's king
    std::vector<Position*> getKingBlockableSquares(const short t_player) const;

    void markPlayerInCheckmate(const short t_player);
    void markPlayerNotInCheckmate(const short t_player);
    bool isPlayerInCheckmate(const short t_player) const;
    bool isPlayerInCheck(const short t_player) const;
    bool isTerminal() const;
    TeamType getWinner() const;

    std::vector<std::deque<Position*>> getLinesAttackingSquare(Position* t_position) const;

    void initialiseAttDefVectors();
    void initialiseActiveVectorsForPiece(Piece* t_piece);
    void initialisePassiveVectorsForPiece(Piece* t_piece);
    void initialiseActiveVectorsForPieceThroughPosition(Piece *t_piece, const Position* t_position);

    std::vector<std::vector<Position*>> getActiveLinesFromVortex(const Piece* t_piece) const;
    std::vector<Position*> getAttackingOrDefendingVectorAlongLine(const Piece* t_piece, const short t_direction) const;
    std::vector<Position*> getAttackingLineOnKingThroughPosition(const Position* t_throughPosition, const short t_owner) const;
    std::vector<Piece*> getPawnReplaceOptionsForPlayer(const short t_player) const;

    void applyMove(const Move* m, const EvaluationMode mode);
    // Applies a move and recomputes all attacking/defending vectors
    void applyMoveDeNovo(const Move* m);
    // Applies a move and only recomputes attacking/defending vectors that are affected by the move
    void applyMoveCumulative(const Move* m);

    void undoMove(const Move* m, const EvaluationMode mode);
    void undoMoveDeNovo(const Move* m);
    void undoMoveCumulative(const Move* m);
    void updateCheckmateRecords();

    int getNumPieces() const;

private:
    // Each element denotes a position (indexed 0 - 157)
    // These don't map to anything, but they're helpful for avoiding creating multiple copies of the same position in memory
    std::vector<Position*> m_positions;

    // Each element denotes a Piece* or a nullptr (indexed 0 - 157)
    // Maps positions to the pieces on them
    // Useful for checking if a square contains a piece (and which piece it is)
    std::vector<Piece*> m_squares;

    // 2D Array: First layer denotes player, second layer denotes pieces
    // Second layer indexed:
    //    0 - 4: Pawns
    //    5: Bishop 1
    //    6: Knight
    //    7: Queen
    //    8: Bishop 2
    //    9: Duchess
    //    10: Rook 1
    //    11: Wizard
    //    12: King
    //    13: Fortress
    //    14: Rook 2
    std::vector<std::vector<Piece*>> m_pieces;

    std::vector<bool> m_checkmateRecords;

};

#endif //DUCHESS_BOARD_H
