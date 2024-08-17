#pragma once

#include "ChessEngine/common.h"

namespace masks {

struct StraightRays {
    bitmask north;
    bitmask east;
    bitmask south;
    bitmask west;
};

struct DiagonalRays {
    bitmask northEast;
    bitmask southEast;
    bitmask southWest;
    bitmask northWest;
};

bitmask getNorthRay(const int ind);
bitmask getNorthEastRay(const int ind);
bitmask getEastRay(const int ind);
bitmask getSouthEastRay(const int ind);
bitmask getSouthRay(const int ind);
bitmask getSouthWestRay(const int ind);
bitmask getWestRay(const int ind);
bitmask getNorthWestRay(const int ind);

StraightRays getStraightRays(const int ind);
DiagonalRays getDiagonalRays(const int ind);

const std::vector<StraightRays> getAllStraightRayBitmasks();
const std::vector<DiagonalRays> getAllDiagonalRayBitmasks();

} // namespace masks