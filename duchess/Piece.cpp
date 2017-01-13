#include "Position.h"
#include "Piece.h"
#include "PieceType.h"
#include "MoveVector.h"
#include <algorithm>
#include <sstream>
#include <iostream>

void Piece::updatePosition(Position* p) {
    // TODO: Update attacking and defending lines
    this->m_position = p;
}

bool Piece::operator==(const Piece &t_otherPiece) const {
    return this->getOwner() == t_otherPiece.getOwner() &&
            this->getType() == t_otherPiece.getType() &&
            *(this->getPosition()) == *(t_otherPiece.getPosition());
}

// TODO: Reuse above?
bool Piece::operator!=(const Piece &t_otherPiece) const {
    return !(this->getOwner() == t_otherPiece.getOwner() &&
           this->getType() == t_otherPiece.getType() &&
           *(this->getPosition()) == *(t_otherPiece.getPosition()));
}

std::string Piece::toString() const {
    std::stringstream result;

    std::string type;
    const PieceType thisType = this->getType();
    if (thisType == PieceType::PAWN) {
        type = "PAWN";
    } else if (thisType == PieceType::ROOK) {
        type = "ROOK";
    } else if (thisType == PieceType::BISHOP) {
        type = "BISHOP";
    } else if (thisType == PieceType::KNIGHT) {
        type = "KNIGHT";
    } else if (thisType == PieceType::QUEEN) {
        type = "QUEEN";
    } else if (thisType == PieceType::KING) {
        type = "KING";
    } else if (thisType == PieceType::FORTRESS) {
        type = "FORTRESS";
    } else if (thisType == PieceType::DUCHESS) {
        type = "DUCHESS";
    } else if (thisType == PieceType::WIZARD) {
        type = "WIZARD";
    } else {
        type = "UNKNOWN";
    }

    result << type << " at " << this->getPosition()->toString() << " owned by " << this->getOwner();

    return result.str();
}

std::string Piece::getMoveVectorsAsString() const {
    std::stringstream result;

    result << "Active attacking vectors (" << this->getActiveAttackingVectors().size() << " found)\n";
    std::vector<std::string> actAttVec;
    for (auto const& mv : this->getActiveAttackingVectors()) {
        actAttVec.push_back(mv->toString());
    }
    std::sort(actAttVec.begin(), actAttVec.end());
    for (auto const& vecStr : actAttVec) {
        result << vecStr;
    }
    result << "\n";

    result << "Active defending vectors (" << this->getActiveDefendingVectors().size() << " found)\n";
    std::vector<std::string> actDefVec;
    for (auto const& mv : this->getActiveDefendingVectors()) {
        actDefVec.push_back(mv->toString());
    }
    std::sort(actDefVec.begin(), actDefVec.end());
    for (auto const& vecStr : actDefVec) {
        result << vecStr;
    }
    result << "\n";

    result << "Passive attacking vectors (" << this->getPassiveAttackingVectors().size() << " found)\n";
    std::vector<std::string> pasAttVec;
    for (auto const& mv : this->getPassiveAttackingVectors()) {
        pasAttVec.push_back(mv->toString());
    }
    std::sort(pasAttVec.begin(), pasAttVec.end());
    for (auto const& vecStr : pasAttVec) {
        result << vecStr;
    }
    result << "\n";

    result << "Passive defending vectors (" << this->getPassiveDefendingVectors().size() << " found)\n";
    std::vector<std::string> pasDefVec;
    for (auto const& mv : this->getPassiveDefendingVectors()) {
        pasDefVec.push_back(mv->toString());
    }
    std::sort(pasDefVec.begin(), pasDefVec.end());
    for (auto const& vecStr : pasDefVec) {
        result << vecStr;
    }

    return result.str();
}

void Piece::clearActiveMoveVectors() {
    for (auto& mv : m_activeAttackingVectors) {
        mv->deregisterSelfFromPassivePiece();
        delete mv;
    }

    for (auto& mv : m_activeDefendingVectors) {
        mv->deregisterSelfFromPassivePiece();
        delete mv;
    }

    m_activeAttackingVectors.clear();
    m_activeDefendingVectors.clear();
}

void Piece::clearPassiveMoveVectors() {
    for (auto& mv : m_passiveBeingAttacked) {
        mv->deregisterSelfFromActivePiece();
        delete mv;
    }

    for (auto& mv : m_passiveBeingDefended) {
        mv->deregisterSelfFromActivePiece();
        delete mv;
    }

    m_passiveBeingAttacked.clear();
    m_passiveBeingDefended.clear();
}

void Piece::clearAllMoveVectors() {
    this->clearActiveMoveVectors();
    this->clearPassiveMoveVectors();
}

// TODO: This will run into problems when it needs to tackle more than one pos
// TODO check if getter worked
void Piece::clearActiveMoveVectorsThatPassThroughPosition(Position* t_position) {
    std::vector<MoveVector*> activeAttVecs = this->getActiveAttackingVectors();
    std::vector<MoveVector*> activeDefVecs = this->getActiveDefendingVectors();
    for (auto& mv : activeAttVecs) {
        if (mv->doesCrossOver(t_position)) {
            mv->deregisterSelf();
            delete mv;
        }
    }

    for (auto& mv : activeDefVecs) {
        if (mv->doesCrossOver(t_position)) {
            mv->deregisterSelf();
            delete mv;
        }
    }

}

void Piece::deleteActiveMoveVectors() {
    for (auto mv : m_activeAttackingVectors) {
        delete mv;
    }

    for (auto mv : m_activeDefendingVectors) {
        delete mv;
    }
}

void Piece::eraseMoveVector(MoveVector* t_moveVector) {
    const bool amIActive = t_moveVector->getActivePiece() == this;
    const MoveVectorType moveVectorType = t_moveVector->getMoveVectorType();
    std::vector<MoveVector*>::iterator it;
    if (amIActive) {
        if (moveVectorType == MoveVectorType::ATTACKING) {
            it = std::find(m_activeAttackingVectors.begin(), m_activeAttackingVectors.end(), t_moveVector);
            if (it != m_activeAttackingVectors.end()) {
                m_activeAttackingVectors.erase(it);
            }
        } else {
            it = std::find(m_activeDefendingVectors.begin(), m_activeDefendingVectors.end(), t_moveVector);
            if (it != m_activeDefendingVectors.end()) {
                m_activeDefendingVectors.erase(it);
            }
        }
    } else {
        if (moveVectorType == MoveVectorType::ATTACKING) {
            it = std::find(m_passiveBeingAttacked.begin(), m_passiveBeingAttacked.end(), t_moveVector);
            if (it != m_passiveBeingAttacked.end()) {
                m_passiveBeingAttacked.erase(it);
            }
        } else {
            it = std::find(m_passiveBeingDefended.begin(), m_passiveBeingDefended.end(), t_moveVector);
            if (it != m_passiveBeingDefended.end()) {
                m_passiveBeingDefended.erase(it);
            }
        }
    }

}

void Piece::addMoveVector(MoveVector* t_moveVector) {
    const bool amIActive = t_moveVector->getActivePiece() == this;
    const MoveVectorType moveVectorType = t_moveVector->getMoveVectorType();
    if (amIActive) {
        if (moveVectorType == MoveVectorType::ATTACKING) {
            m_activeAttackingVectors.push_back(t_moveVector);
        } else {
            m_activeDefendingVectors.push_back(t_moveVector);
        }
    } else {
        if (moveVectorType == MoveVectorType::ATTACKING) {
            m_passiveBeingAttacked.push_back(t_moveVector);
        } else {
            m_passiveBeingDefended.push_back(t_moveVector);
        }
    }
}

std::vector<Piece*> Piece::getPiecesActivelyTouchingThis() const {
    std::vector<Piece*> pieces;

    for (auto const& mv : m_passiveBeingAttacked) {
        pieces.push_back(mv->getActivePiece());
    }

    for (auto const& mv : m_passiveBeingDefended) {
        pieces.push_back(mv->getActivePiece());
    }

    return pieces;
}

Piece::~Piece() {
    this->deleteActiveMoveVectors();
}

std::string Piece::getInitial() const {
    const PieceType p = this->getType();
    if (p == PieceType::PAWN) {
        return "P";
    } else if (p == PieceType::BISHOP) {
        return "B";
    } else if (p == PieceType::KNIGHT) {
        return "N";
    } else if (p == PieceType::DUCHESS) {
        return "D";
    } else if (p == PieceType::FORTRESS) {
        return "F";
    } else if (p == PieceType::QUEEN) {
        return "Q";
    } else if (p == PieceType::ROOK) {
        return "R";
    } else if (p == PieceType::KING) {
        return "K";
    } else if (p == PieceType::WIZARD) {
        return "W";
    } else {
        return "?";
    }
}