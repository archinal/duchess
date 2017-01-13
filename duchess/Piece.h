#ifndef DUCHESS_PIECE_H
#define DUCHESS_PIECE_H

#include "Position.h"
#include "PieceType.h"
#include "MoveVectorType.h"
#include <vector>
#include <string>

// Forward declare move vector to prevent circular hash includes
class MoveVector;

class Piece {
public:

    Piece(Position* t_position, const PieceType t_type, const short t_owner) :
            m_position(t_position), m_type(t_type), m_owner(t_owner)
            { }

    ~Piece();

    Position* getPosition() const {
        return m_position;
    }

    PieceType getType() const {
        return m_type;
    }

    short getOwner() const {
        return m_owner;
    }

    void updatePosition(Position* p);

    bool operator==(const Piece& t_otherPiece) const;
    bool operator!=(const Piece& t_otherPiece) const;

    std::string toString() const;

    std::string getMoveVectorsAsString() const;

    std::vector<MoveVector*> getActiveAttackingVectors() const {
        return m_activeAttackingVectors;
    }

    std::vector<MoveVector*> getActiveDefendingVectors() const {
        return m_activeDefendingVectors;
    }

    std::vector<MoveVector*> getPassiveAttackingVectors() const {
        return m_passiveBeingAttacked;
    }

    std::vector<MoveVector*> getPassiveDefendingVectors() const {
        return m_passiveBeingDefended;
    }

    std::vector<Piece*> getPiecesActivelyTouchingThis() const;

    // These clear functions are responsible for deleting the allocated MoveVector memory
    // Deletes all move vectors that I'm a part of (and deletes them from the other affected piece as well)
    void clearAllMoveVectors();
    void clearActiveMoveVectors();
    void clearPassiveMoveVectors();
    // Deletes all its move vectors that pass through a square. To be used when a piece effectively blocks off an old vector
    void clearActiveMoveVectorsThatPassThroughPosition(Position* t_position);

    // Just deletes my active vectors
    // To be used by Piece destructor only
    void deleteActiveMoveVectors();

    // Deletes a specific move vector from me only
    // Should only be called by MoveVector::deregisterSelf
    void eraseMoveVector(MoveVector* t_moveVector);
    // Should only be called by MoveVector::registerSelf
    void addMoveVector(MoveVector* t_moveVector);

    std::string getInitial() const;


private:
    // TODO: Can I indicate that the value inside this pointer won't change, just that the pointer address might?
    Position* m_position;
    const PieceType m_type;
    const short m_owner;

    // Two way binding because we don't know who will move first
    // Vector of AttackVectors for who I am attacking
    std::vector<MoveVector*> m_activeAttackingVectors;
    // Vector of AttackVectors for who is attacking me
    std::vector<MoveVector*> m_passiveBeingAttacked;

    // Vector of DefenceVectors for who I am defending
    std::vector<MoveVector*> m_activeDefendingVectors;
    // Vector of DefenceVectors for who is defending me
    std::vector<MoveVector*> m_passiveBeingDefended;
};

#endif //DUCHESS_PIECE_H
