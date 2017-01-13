#ifndef DUCHESS_CPP_MOVEVECTOR_H
#define DUCHESS_CPP_MOVEVECTOR_H

#include <vector>
#include <cassert>

#include "MoveVectorType.h"
#include "Piece.h"
#include "PieceType.h"
#include "Position.h"
#include "TeamUtils.h"

class MoveVector {
public:
    MoveVector(Piece* t_activePiece, Piece* t_passivePiece, const std::vector<Position*> t_positions) :
            m_activePiece(t_activePiece), m_passivePiece(t_passivePiece), m_positions(t_positions),
            m_moveVectorType(TeamUtils::isSameTeam(t_activePiece->getOwner(), t_passivePiece->getOwner()) ? MoveVectorType::DEFENDING : MoveVectorType::ATTACKING)
            {
                assert(*t_activePiece != *t_passivePiece);
            }

    // Does this move vector involve t_piece as an active or passive piece?
    bool doesInvolve(Piece* t_piece) const;

    // Does this move vector cross over a certain position?
    bool doesCrossOver(Position* t_position) const;

    PieceType getActivePieceType() const {
        return this->getActivePiece()->getType();
    }

    PieceType getPassivePieceType() const {
        return this->getPassivePiece()->getType();
    }

    Piece* getActivePiece() const {
        return m_activePiece;
    }

    Piece* getPassivePiece() const {
        return m_passivePiece;
    }

    const std::vector<Position*>& getPositions() const {
        return m_positions;
    }

    MoveVectorType getMoveVectorType() const {
        return m_moveVectorType;
    }

    // Adds itself to the lists maintained by each affected piece
    void registerSelf();
    // Removes itself from the lists maintained by each affected piece
    void deregisterSelf();
    void deregisterSelfFromActivePiece();
    void deregisterSelfFromPassivePiece();

    std::string toString() const;

private:
    // TODO: Can any of these be const?

    Piece* m_activePiece;
    Piece* m_passivePiece;

    // A vector of positions leading from the active piece to the passive piece
    // e.g. representing a diagonal line connecting a bishop to a knight
    const std::vector<Position*> m_positions;

    const MoveVectorType m_moveVectorType;

};

#endif //DUCHESS_CPP_MOVEVECTOR_H
