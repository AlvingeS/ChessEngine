#include "ChessEngine/src/game/masks/RayBitmasks.h"

#include "ChessEngine/src/utils/ChessUtils.h"

namespace masks {

bitmask getNorthRay(int ind) 
{
    int rank = utils::rankFromBitIndex(ind);
    int file = utils::fileFromBitIndex(ind);

    bitmask north_ray = 0ULL;

    for (int i = rank + 1; i < 8; i++) {
        north_ray |= (1ULL << (i * 8 + file));
    }

    return north_ray;
}

bitmask getNorthEastRay(int ind) 
{
    int rank = utils::rankFromBitIndex(ind);
    int file = utils::fileFromBitIndex(ind);

    bitmask north_east_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank + i < 8 && file - i >= 0) {
            north_east_ray |= (1ULL << ((rank + i) * 8 + file - i));
        }
    }

    return north_east_ray;
}

bitmask getEastRay(int ind) 
{
    int rank = utils::rankFromBitIndex(ind);
    int file = utils::fileFromBitIndex(ind);

    bitmask east_ray = 0ULL;

    for (int i = file - 1; i >= 0; i--) {
        east_ray |= (1ULL << (rank * 8 + i));
    }

    return east_ray;
}

bitmask getSouthEastRay(int ind) 
{
    int rank = utils::rankFromBitIndex(ind);
    int file = utils::fileFromBitIndex(ind);

    bitmask south_east_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank - i >= 0 && file - i >= 0) {
            south_east_ray |= (1ULL << ((rank - i) * 8 + file - i));
        }
    }

    return south_east_ray;
}

bitmask getSouthRay(int ind) 
{
    int rank = utils::rankFromBitIndex(ind);
    int file = utils::fileFromBitIndex(ind);

    bitmask south_ray = 0ULL;

    for (int i = rank - 1; i >= 0; i--) {
        south_ray |= (1ULL << (i * 8 + file));
    }

    return south_ray;
}

bitmask getSouthWestRay(int ind) 
{
    int rank = utils::rankFromBitIndex(ind);
    int file = utils::fileFromBitIndex(ind);

    bitmask south_west_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank - i >= 0 && file + i < 8) {
            south_west_ray |= (1ULL << ((rank - i) * 8 + file + i));
        }
    }

    return south_west_ray;
}

bitmask getWestRay(int ind) 
{
    int rank = utils::rankFromBitIndex(ind);
    int file = utils::fileFromBitIndex(ind);

    bitmask west_ray = 0ULL;

    for (int i = file + 1; i < 8; i++) {
        west_ray |= (1ULL << (rank * 8 + i));
    }

    return west_ray;
}

bitmask getNorthWestRay(int ind) 
{
    int rank = utils::rankFromBitIndex(ind);
    int file = utils::fileFromBitIndex(ind);

    bitmask north_west_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank + i < 8 && file + i < 8) {
            north_west_ray |= (1ULL << ((rank + i) * 8 + file + i));
        }
    }

    return north_west_ray;
}

StraightRays getStraightRays(int ind) 
{
    return {
        getNorthRay(ind),
        getEastRay(ind),
        getSouthRay(ind),
        getWestRay(ind)
    };
}

DiagonalRays getDiagonalRays(int ind) 
{
    return {
        getNorthEastRay(ind),
        getSouthEastRay(ind),
        getSouthWestRay(ind),
        getNorthWestRay(ind)
    };
}

std::vector<StraightRays> getAllStraightRayBitmasks() 
{
    std::vector<StraightRays> allStraightRayBitmasks;

    for (int i = 0; i < 64; i++) {
        allStraightRayBitmasks.push_back(getStraightRays(i));
    }

    return allStraightRayBitmasks;
}

std::vector<DiagonalRays> getAllDiagonalRayBitmasks() 
{
    std::vector<DiagonalRays> allDiagonalRayBitmasks;

    for (int i = 0; i < 64; i++) {
        allDiagonalRayBitmasks.push_back(getDiagonalRays(i));
    }

    return allDiagonalRayBitmasks;
}

} // namespace masks