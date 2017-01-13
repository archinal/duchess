#include "MoveVector.h"
#include "Position.h"
#include "Piece.h"

#include <string>
#include <sstream>

// Does this move vector involve t_piece as an active or passive piece?
bool MoveVector::doesInvolve(Piece* t_piece) const {
    return t_piece == this->getActivePiece() || t_piece == this->getPassivePiece();
}

// Does this move vector cross over a certain position?
bool MoveVector::doesCrossOver(Position* t_position) const {
    for (auto const& position : this->getPositions()) {
        if (*position == *t_position) {
            return true;
        }
    }
    return false;
}

void MoveVector::registerSelf() {
    // Active
    this->getActivePiece()->addMoveVector(this);

    // Passive
    this->getPassivePiece()->addMoveVector(this);
}

void MoveVector::deregisterSelf() {
    this->deregisterSelfFromPassivePiece();
    this->deregisterSelfFromActivePiece();
}

void MoveVector::deregisterSelfFromPassivePiece() {
    // Remove myself from the passive piece
    this->getPassivePiece()->eraseMoveVector(this);
}

void MoveVector::deregisterSelfFromActivePiece() {
    // Remove myself from the active piece
    this->getActivePiece()->eraseMoveVector(this);
}

std::string MoveVector::toString() const {
    std::stringstream result;

    result << "Active piece: " << this->getActivePiece()->toString() << "\n";

    result << "Passive piece: " << this->getPassivePiece()->toString() << "\n";

    result << "Positions: {";

    for (auto const& pos : this->getPositions()) {
        result << " " << pos->toString();
    }

    result << " }\n";

    return result.str();
}
