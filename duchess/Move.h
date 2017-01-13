#ifndef DUCHESS_MOVE_H
#define DUCHESS_MOVE_H

#include "Position.h"
#include "Piece.h"

#include <string>

class Move {
public:
    Move() : m_from(nullptr), m_to(nullptr), m_pieceTaken(nullptr), m_replacePiece(nullptr), m_isNull(true) {};
    Move(const Position* t_from, const Position* t_to, Piece* t_pieceTaken = nullptr, Piece* t_replacePiece = nullptr, const bool t_isNullMove = false) :
            m_from(t_from), m_to(t_to), m_pieceTaken(t_pieceTaken), m_replacePiece(t_replacePiece), m_isNull(t_isNullMove) {};

    const Position* getFromPosition() const {
        return m_from;
    }

    const Position* getToPosition() const {
        return m_to;
    }

    Piece* getReplacePiece() const {
        return m_replacePiece;
    }

    Piece* getPieceTaken() const {
        return m_pieceTaken;
    }

    bool isNull() const {
        return m_isNull;
    }

    std::string toString() const;
    std::string toShortString() const;

    bool operator==(const Move& t_otherMove) const;
    bool operator!=(const Move& t_otherMove) const;
    bool operator<(const Move& t_otherMove) const;

private:
    // The start position of the move (contains the operative piece)
    const Position* m_from;

    // The end position of the move
    const Position* m_to;

    // If this moves takes a piece, record which piece that was
    // TODO: I'm pretty sure these can't be const (since the data in Piece may change), but I could be wrong
    Piece* m_pieceTaken;

    // If this move upgrades a pawn on the vortex, record the piece that replaced it
    Piece* m_replacePiece;

    // Used to denote null moves when in checkmate or stalemate
    bool m_isNull;

};

#endif //DUCHESS_MOVE_H
