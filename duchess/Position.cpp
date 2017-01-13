#include "Position.h"
#include "PositionUtils.h"

#include <sstream>

std::string Position::toString() const {
    std::stringstream result;
    if (this->isVortex()) {
        result << "V";
    } else if (this->isOffBoard()) {
        result << "OB";
    } else {
        result << this->getFlap() << this->getRow() << this->getColumn();
    }

    return result.str();
}

Position::Position(short t_flap, char t_row, short t_column, bool t_offBoard, bool t_vortex) {
    this->m_offBoard = t_offBoard;
    this->m_vortex = t_vortex;

    // Sanitization to ensure canonical position used
    if ((t_row == 'a' || t_row == 'b') && t_column >= 5) {
        const short newColumn = (t_row == 'a' ? 5 : 6);
        const char newRow = (t_column == 5 ? 'e' : 'd');
        const short newFlap = (t_flap == 6 ? 1 : t_flap + 1);
        this->m_flap = newFlap;
        this->m_row = newRow;
        this->m_column = newColumn;
    } else {
        this->m_flap = t_flap;
        this->m_row = t_row;
        this->m_column = t_column;
    }

    m_boardIndex = PositionUtils::positionToBoardIndexSixPlayerSlow(this);
}

Position::Position(const std::string t_posStr) {
    short flap = 0;
    char row = '\0';
    short column = 0;

    const bool offBoard = (t_posStr == "OB");
    const bool vortex = (t_posStr == "V");

    if (!(offBoard || vortex)) {
        flap = t_posStr[0] - '0';
        row = t_posStr[1];
        column = t_posStr[2] - '0';
    }

    // Sanitization to ensure canonical position used
    if ((row == 'a' || row == 'b') && column >= 5) {
        const short newColumn = (row == 'a' ? 5 : 6);
        const char newRow = (column == 5 ? 'e' : 'd');
        const short newFlap = (flap == 6 ? 1 : flap + 1);
        this->m_flap = newFlap;
        this->m_row = newRow;
        this->m_column = newColumn;
    } else {
        this->m_flap = flap;
        this->m_row = row;
        this->m_column = column;
    }

    this->m_offBoard = offBoard;
    this->m_vortex = vortex;

    m_boardIndex = PositionUtils::positionToBoardIndexSixPlayerSlow(this);
}

bool Position::operator==(const Position &t_otherPosition) const {
    return this->getBoardIndex() == t_otherPosition.getBoardIndex();
}

bool Position::operator!=(const Position &t_otherPosition) const {
    return this->getBoardIndex() != t_otherPosition.getBoardIndex();
}
