
#include "BitBasics.h"
#include <vector>

namespace bits {
    U64 getFileMask(int file);
    U64 getRankMask(int rank);
    U64 getNorthRay(int ind);
    U64 getNorthEastRay(int ind);
    U64 getEastRay(int ind);
    U64 getSouthEastRay(int ind);
    U64 getSouthRay(int ind);
    U64 getSouthWestRay(int ind);
    U64 getWestRay(int ind);
    U64 getNorthWestRay(int ind);
    std::vector<U64> getStraightRays(int ind);
    std::vector<U64> getDiagonalRays(int ind);
    std::vector<std::vector<U64>> getAllStraightRayBitMasks();
    std::vector<std::vector<U64>> getAllDiagonalRayBitMasks();
    std::vector<std::vector<U64>> getAllKnightAttackBitMasks();
    std::vector<std::vector<U64>> getAllKingAttackBitMasks();
}