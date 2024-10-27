#pragma once

#include "ChessEngine/src/common.h"

namespace logic {
namespace movegen {
namespace bitmasks {

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

bitmask getNorthRay(int ind);
bitmask getNorthEastRay(int ind);
bitmask getEastRay(int ind);
bitmask getSouthEastRay(int ind);
bitmask getSouthRay(int ind);
bitmask getSouthWestRay(int ind);
bitmask getWestRay(int ind);
bitmask getNorthWestRay(int ind);

StraightRays getStraightRays(int ind);
DiagonalRays getDiagonalRays(int ind);

std::vector<StraightRays> getAllStraightRayBitmasks();
std::vector<DiagonalRays> getAllDiagonalRayBitmasks();

} // namespace bitmasks
} // namespace movegen
} // namespace logic