#include "GeneralUtils.h"
#include "Position.h"
#include <vector>

std::vector<BoardIndex> GeneralUtils::mergeArrays(const std::vector<BoardIndex> a, const std::vector<BoardIndex> b) {
    // Merge the two results into one vector
    std::vector<BoardIndex> result;

    result.reserve(a.size() + b.size()); // preallocate memory
    result.insert(result.end(), a.begin(), a.end());
    result.insert(result.end(), b.begin(), b.end());

    return result;
}