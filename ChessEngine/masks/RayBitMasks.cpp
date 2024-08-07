#include "RayBitMasks.h"

#include "ChessEngine/utils/ChessUtils.h"

namespace masks {
U64 getNorthRay(const int ind) 
{
    int rank = utils::rankFromBitIndex(ind);
    int file = utils::fileFromBitIndex(ind);

    U64 north_ray = 0ULL;

    for (int i = rank + 1; i < 8; i++) {
        north_ray |= (1ULL << (i * 8 + file));
    }

    return north_ray;
}

U64 getNorthEastRay(const int ind) 
{
    int rank = utils::rankFromBitIndex(ind);
    int file = utils::fileFromBitIndex(ind);

    U64 north_east_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank + i < 8 && file - i >= 0) {
            north_east_ray |= (1ULL << ((rank + i) * 8 + file - i));
        }
    }

    return north_east_ray;
}

U64 getEastRay(const int ind) 
{
    int rank = utils::rankFromBitIndex(ind);
    int file = utils::fileFromBitIndex(ind);

    U64 east_ray = 0ULL;

    for (int i = file - 1; i >= 0; i--) {
        east_ray |= (1ULL << (rank * 8 + i));
    }

    return east_ray;
}

U64 getSouthEastRay(const int ind) 
{
    int rank = utils::rankFromBitIndex(ind);
    int file = utils::fileFromBitIndex(ind);

    U64 south_east_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank - i >= 0 && file - i >= 0) {
            south_east_ray |= (1ULL << ((rank - i) * 8 + file - i));
        }
    }

    return south_east_ray;
}

U64 getSouthRay(const int ind) 
{
    int rank = utils::rankFromBitIndex(ind);
    int file = utils::fileFromBitIndex(ind);

    U64 south_ray = 0ULL;

    for (int i = rank - 1; i >= 0; i--) {
        south_ray |= (1ULL << (i * 8 + file));
    }

    return south_ray;
}

U64 getSouthWestRay(const int ind) 
{
    int rank = utils::rankFromBitIndex(ind);
    int file = utils::fileFromBitIndex(ind);

    U64 south_west_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank - i >= 0 && file + i < 8) {
            south_west_ray |= (1ULL << ((rank - i) * 8 + file + i));
        }
    }

    return south_west_ray;
}

U64 getWestRay(const int ind) 
{
    int rank = utils::rankFromBitIndex(ind);
    int file = utils::fileFromBitIndex(ind);

    U64 west_ray = 0ULL;

    for (int i = file + 1; i < 8; i++) {
        west_ray |= (1ULL << (rank * 8 + i));
    }

    return west_ray;
}

U64 getNorthWestRay(const int ind) 
{
    int rank = utils::rankFromBitIndex(ind);
    int file = utils::fileFromBitIndex(ind);

    U64 north_west_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank + i < 8 && file + i < 8) {
            north_west_ray |= (1ULL << ((rank + i) * 8 + file + i));
        }
    }

    return north_west_ray;
}

StraightRays getStraightRays(const int ind) 
{
    return {
        getNorthRay(ind),
        getEastRay(ind),
        getSouthRay(ind),
        getWestRay(ind)
    };
}

DiagonalRays getDiagonalRays(const int ind) 
{
    return {
        getNorthEastRay(ind),
        getSouthEastRay(ind),
        getSouthWestRay(ind),
        getNorthWestRay(ind)
    };
}

const std::vector<StraightRays> getAllStraightRayBitmasks() 
{
    std::vector<StraightRays> allStraightRayBitmasks;

    for (int i = 0; i < 64; i++) {
        allStraightRayBitmasks.push_back(getStraightRays(i));
    }

    return allStraightRayBitmasks;
}

const std::vector<DiagonalRays> getAllDiagonalRayBitmasks() 
{
    std::vector<DiagonalRays> allDiagonalRayBitmasks;

    for (int i = 0; i < 64; i++) {
        allDiagonalRayBitmasks.push_back(getDiagonalRays(i));
    }

    return allDiagonalRayBitmasks;
}

} // namespace masks