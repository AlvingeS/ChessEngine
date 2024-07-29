#pragma once

#include "ChessEngine/common.h"

namespace masks {

struct StraightRays {
    U64 north;
    U64 east;
    U64 south;
    U64 west;
};

struct DiagonalRays {
    U64 northEast;
    U64 southEast;
    U64 southWest;
    U64 northWest;
};

U64 getNorthRay(const int ind);
U64 getNorthEastRay(const int ind);
U64 getEastRay(const int ind);
U64 getSouthEastRay(const int ind);
U64 getSouthRay(const int ind);
U64 getSouthWestRay(const int ind);
U64 getWestRay(const int ind);
U64 getNorthWestRay(const int ind);

StraightRays getStraightRays(const int ind);
DiagonalRays getDiagonalRays(const int ind);

const std::vector<StraightRays> getAllStraightRayBitmasks();
const std::vector<DiagonalRays> getAllDiagonalRayBitmasks();

} // namespace masks