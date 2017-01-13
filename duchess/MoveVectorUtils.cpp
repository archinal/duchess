#include "MoveVectorUtils.h"
#include "MoveVector.h"
#include "Position.h"

#include <vector>
#include <algorithm>

std::vector<Position*> MoveVectorUtils::getCommonPositions(const std::vector<MoveVector*>& t_moveVectors) {
    std::vector<Position*> positions;
    if (t_moveVectors.size() > 0) {
        positions = t_moveVectors[0]->getPositions();

        for (std::vector<MoveVector*>::const_iterator it = t_moveVectors.begin() + 1; it != t_moveVectors.end(); ++it) {
            auto mv = *it;
            positions = MoveVectorUtils::getCommonPositions(positions, mv->getPositions());
        }

    }


    return positions;
}

std::vector<Position*> MoveVectorUtils::getCommonPositions(const std::vector<Position*>& t_positions1, const std::vector<Position*>& t_positions2) {
    std::vector<Position*> positions;

    for (auto const& pos : t_positions1) {
        if (std::find(t_positions2.begin(), t_positions2.end(), pos) != t_positions2.end()) {
            positions.push_back(pos);
        }
    }

    return positions;
}