#ifndef DUCHESS_CPP_MOVEVECTORUTILS_H
#define DUCHESS_CPP_MOVEVECTORUTILS_H

#include "MoveVector.h"
#include "Position.h"

#include <vector>

class MoveVectorUtils {
public:
    // Given a list of move vectors, which Positions do they all contain (if any)?
    static std::vector<Position*> getCommonPositions(const std::vector<MoveVector*>& t_moveVectors);
    static std::vector<Position*> getCommonPositions(const std::vector<Position*>& t_positions1, const std::vector<Position*>& t_positions2);
};

#endif //DUCHESS_CPP_MOVEVECTORUTILS_H
