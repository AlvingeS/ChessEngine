#include "logic/movegen/bitmasks/RayBitmasks.h"

#include "logic/movegen/utils/ChessUtils.h"

namespace logic {

bitmask RayBitmasks::getNorthRay(int ind) 
{
    int rank = ChessUtils::rankFromBitIndex(ind);
    int file = ChessUtils::fileFromBitIndex(ind);

    bitmask north_ray = 0ULL;

    for (int i = rank + 1; i < 8; i++) {
        north_ray |= (1ULL << (i * 8 + file));
    }

    return north_ray;
}

bitmask RayBitmasks::getNorthEastRay(int ind) 
{
    int rank = ChessUtils::rankFromBitIndex(ind);
    int file = ChessUtils::fileFromBitIndex(ind);

    bitmask north_east_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank + i < 8 && file - i >= 0) {
            north_east_ray |= (1ULL << ((rank + i) * 8 + file - i));
        }
    }

    return north_east_ray;
}

bitmask RayBitmasks::getEastRay(int ind) 
{
    int rank = ChessUtils::rankFromBitIndex(ind);
    int file = ChessUtils::fileFromBitIndex(ind);

    bitmask east_ray = 0ULL;

    for (int i = file - 1; i >= 0; i--) {
        east_ray |= (1ULL << (rank * 8 + i));
    }

    return east_ray;
}

bitmask RayBitmasks::getSouthEastRay(int ind) 
{
    int rank = ChessUtils::rankFromBitIndex(ind);
    int file = ChessUtils::fileFromBitIndex(ind);

    bitmask south_east_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank - i >= 0 && file - i >= 0) {
            south_east_ray |= (1ULL << ((rank - i) * 8 + file - i));
        }
    }

    return south_east_ray;
}

bitmask RayBitmasks::getSouthRay(int ind) 
{
    int rank = ChessUtils::rankFromBitIndex(ind);
    int file = ChessUtils::fileFromBitIndex(ind);

    bitmask south_ray = 0ULL;

    for (int i = rank - 1; i >= 0; i--) {
        south_ray |= (1ULL << (i * 8 + file));
    }

    return south_ray;
}

bitmask RayBitmasks::getSouthWestRay(int ind) 
{
    int rank = ChessUtils::rankFromBitIndex(ind);
    int file = ChessUtils::fileFromBitIndex(ind);

    bitmask south_west_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank - i >= 0 && file + i < 8) {
            south_west_ray |= (1ULL << ((rank - i) * 8 + file + i));
        }
    }

    return south_west_ray;
}

bitmask RayBitmasks::getWestRay(int ind) 
{
    int rank = ChessUtils::rankFromBitIndex(ind);
    int file = ChessUtils::fileFromBitIndex(ind);

    bitmask west_ray = 0ULL;

    for (int i = file + 1; i < 8; i++) {
        west_ray |= (1ULL << (rank * 8 + i));
    }

    return west_ray;
}

bitmask RayBitmasks::getNorthWestRay(int ind) 
{
    int rank = ChessUtils::rankFromBitIndex(ind);
    int file = ChessUtils::fileFromBitIndex(ind);

    bitmask north_west_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank + i < 8 && file + i < 8) {
            north_west_ray |= (1ULL << ((rank + i) * 8 + file + i));
        }
    }

    return north_west_ray;
}

RayBitmasks::StraightRays RayBitmasks::getStraightRays(int ind) 
{
    return {
        getNorthRay(ind),
        getEastRay(ind),
        getSouthRay(ind),
        getWestRay(ind)
    };
}

RayBitmasks::DiagonalRays RayBitmasks::getDiagonalRays(int ind) 
{
    return {
        getNorthEastRay(ind),
        getSouthEastRay(ind),
        getSouthWestRay(ind),
        getNorthWestRay(ind)
    };
}
std::vector<RayBitmasks::StraightRays> RayBitmasks::getAllStraightRayBitmasks() 
{
    std::vector<RayBitmasks::StraightRays> allStraightRayBitmasks;

    for (int i = 0; i < 64; i++) {
        allStraightRayBitmasks.push_back(getStraightRays(i));
    }

    return allStraightRayBitmasks;
}

std::vector<RayBitmasks::DiagonalRays> RayBitmasks::getAllDiagonalRayBitmasks() 
{
    std::vector<RayBitmasks::DiagonalRays> allDiagonalRayBitmasks;

    for (int i = 0; i < 64; i++) {
        allDiagonalRayBitmasks.push_back(getDiagonalRays(i));
    }

    return allDiagonalRayBitmasks;
}

} // namespace logic