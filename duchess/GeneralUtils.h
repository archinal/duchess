#ifndef DUCHESS_GENERALUTILS_H
#define DUCHESS_GENERALUTILS_H

#include "Position.h"
#include <vector>

class GeneralUtils {
public:
    static std::vector<BoardIndex> mergeArrays(const std::vector<BoardIndex> a, const std::vector<BoardIndex> b);
};

#endif //DUCHESS_GENERALUTILS_H
