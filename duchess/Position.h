#ifndef DUCHESS_POSITION_H
#define DUCHESS_POSITION_H

#include <cstdlib>
#include <string>

typedef unsigned char BoardIndex;

// A position denotes any place on the board
class Position {
public:
    Position(short t_flap, char t_row, short t_column, bool t_offBoard = false, bool t_vortex = false);

    Position(const std::string t_posStr);

    bool operator==(const Position& t_otherPosition) const;
    bool operator!=(const Position& t_otherPosition) const;

    std::string toString() const;

    short getFlap() const {
        return m_flap;
    }

    char getRow() const {
        return m_row;
    }

    short getColumn() const {
        return m_column;
    }

    bool isOffBoard() const {
        return m_offBoard;
    }

    bool isVortex() const {
        return m_vortex;
    }

    BoardIndex getBoardIndex() const {
        return m_boardIndex;
    }

private:

    //flap (1-6) row (a-e) and column (1-7)
    short m_flap;
    char m_row;
    short m_column;
    bool m_offBoard;
    bool m_vortex;
    BoardIndex m_boardIndex;

};


#endif //DUCHESS_POSITION_H
