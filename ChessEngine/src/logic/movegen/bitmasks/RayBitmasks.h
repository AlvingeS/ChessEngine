#pragma once

#include "common.h"

namespace logic {

struct RayBitmasks {

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

    static bitmask getNorthRay(int ind);
    static bitmask getNorthEastRay(int ind);
    static bitmask getEastRay(int ind);
    static bitmask getSouthEastRay(int ind);
    static bitmask getSouthRay(int ind);
    static bitmask getSouthWestRay(int ind);
    static bitmask getWestRay(int ind);
    static bitmask getNorthWestRay(int ind);
    
    static StraightRays getStraightRays(int ind);
    static DiagonalRays getDiagonalRays(int ind);
    
    static std::vector<StraightRays> getAllStraightRayBitmasks();
    static std::vector<DiagonalRays> getAllDiagonalRayBitmasks();
};

} // namespace logic