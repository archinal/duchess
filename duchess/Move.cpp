#include "Move.h"

#include <string>
#include <sstream>

bool Move::operator==(const Move &t_otherMove) const {
    // This assumes the Piece pointers are the same address
    bool equalFrom;
    bool equalTo;
    const Position* fromPos = this->getFromPosition();
    const Position* toPos = this->getToPosition();
    const Position* otherFromPos = t_otherMove.getFromPosition();
    const Position* otherToPos = t_otherMove.getToPosition();

    if (fromPos == nullptr || otherFromPos == nullptr) {
        equalFrom = fromPos == otherFromPos;
    } else {
        equalFrom = fromPos->getBoardIndex() == otherFromPos->getBoardIndex();
    }

    if (toPos == nullptr || otherToPos == nullptr) {
        equalTo = toPos == otherToPos;
    } else {
        equalTo = toPos->getBoardIndex() == otherToPos->getBoardIndex();
    }

    return (equalFrom &&
            equalTo &&
            this->getPieceTaken() == t_otherMove.getPieceTaken() &&
            this->getReplacePiece() == t_otherMove.getReplacePiece() &&
            this->isNull() == t_otherMove.isNull());
}

// TODO: Reuse above?
bool Move::operator!=(const Move &t_otherMove) const {
    bool equalFrom;
    bool equalTo;
    const Position* fromPos = this->getFromPosition();
    const Position* toPos = this->getToPosition();
    const Position* otherFromPos = t_otherMove.getFromPosition();
    const Position* otherToPos = t_otherMove.getToPosition();

    if (fromPos == nullptr || otherFromPos == nullptr) {
        equalFrom = fromPos == otherFromPos;
    } else {
        equalFrom = fromPos->getBoardIndex() == otherFromPos->getBoardIndex();
    }

    if (toPos == nullptr || otherToPos == nullptr) {
        equalTo = toPos == otherToPos;
    } else {
        equalTo = toPos->getBoardIndex() == otherToPos->getBoardIndex();
    }

    return !(equalFrom &&
            equalTo &&
            this->getPieceTaken() == t_otherMove.getPieceTaken() &&
            this->getReplacePiece() == t_otherMove.getReplacePiece() &&
            this->isNull() == t_otherMove.isNull());
}

std::string Move::toString() const {
    std::stringstream result;

    if (this->isNull()) {
        result << "Null move";

    } else {

        result << "Move from " << this->getFromPosition()->toString();
        result << " to " << this->getToPosition()->toString();

        if (this->getPieceTaken() != nullptr) {
            result << " taking " << this->getPieceTaken()->toString();
        }

        if (this->getReplacePiece() != nullptr) {
            result << " being replaced with " << this->getReplacePiece()->toString();
        }
    }

    return result.str();
}

std::string Move::toShortString() const {
    std::stringstream result;

    if (this->isNull()) {
        result << "_";

    } else {

        result << this->getFromPosition()->toString();
        result << "_" << this->getToPosition()->toString();

        if (this->getPieceTaken() != nullptr) {
            result << this->getPieceTaken()->getInitial();
        }

        if (this->getReplacePiece() != nullptr) {
            result << this->getReplacePiece()->getInitial();
        }
    }

    return result.str();
}

/**
 * Checks: Null, From pos, to pos, taken piece, replace piece
 */

bool Move::operator<(const Move& t_otherMove) const {
    if (this->isNull()) {
        return !t_otherMove.isNull();
    } else if (t_otherMove.isNull()) {
        return false;
    } else {
        const Position* fromPos = this->getFromPosition();
        const Position* otherFromPos = t_otherMove.getFromPosition();

        if (fromPos->getBoardIndex() < otherFromPos->getBoardIndex()) {
            return true;
        } else if (fromPos->getBoardIndex() == otherFromPos->getBoardIndex()) {
            const Position* toPos = this->getToPosition();
            const Position* otherToPos = t_otherMove.getToPosition();
            if (toPos->getBoardIndex() < otherToPos->getBoardIndex()) {
                return true;
            } else if (toPos->getBoardIndex() == otherToPos->getBoardIndex()) {
                const Piece* takenPiece = this->getPieceTaken();
                const Piece* otherTakenPiece = t_otherMove.getPieceTaken();
                // Just compare the pointers
                if (takenPiece < otherTakenPiece) {
                    return true;
                } else if (takenPiece == otherTakenPiece) {
                    return this->getReplacePiece() < t_otherMove.getReplacePiece();
                } else {
                    return false;
                }

            } else {
                return false;
            }
        } else {
            return false;
        }
    }
}