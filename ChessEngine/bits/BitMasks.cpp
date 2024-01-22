#include "BitMasks.h"
#include "BitDir.h"

namespace bits {
    constexpr U64 FILE_MASK[8] = {
        0x8080808080808080ULL,
        0x4040404040404040ULL,
        0x2020202020202020ULL,
        0x1010101010101010ULL,
        0x0808080808080808ULL,
        0x0404040404040404ULL,
        0x0202020202020202ULL,
        0x0101010101010101ULL
    };
    
    constexpr U64 RANK_MASK[8] = {
        0x00000000000000FFULL,
        0x000000000000FF00ULL,
        0x0000000000FF0000ULL,
        0x00000000FF000000ULL,
        0x000000FF00000000ULL,
        0x0000FF0000000000ULL,
        0x00FF000000000000ULL,
        0xFF00000000000000ULL
    };
    
    U64 getFileMask(int file) {
        return FILE_MASK[file];
    }

    U64 getRankMask(int rank) {
        return RANK_MASK[rank];
    }

    U64 getNorthRay(int ind) {
        int rank = rankFromBitIndex(ind);
        int file = fileFromBitIndex(ind);

        U64 north_ray = 0ULL;

        for (int i = rank + 1; i < 8; i++) {
            north_ray |= (1ULL << (i * 8 + file));
        }

        return north_ray;
    }

    U64 getNorthEastRay(int ind) {
        int rank = rankFromBitIndex(ind);
        int file = fileFromBitIndex(ind);

        U64 north_east_ray = 0ULL;

        for (int i = 1; i < 8; i++) {
            if (rank + i < 8 && file - i >= 0) {
                north_east_ray |= (1ULL << ((rank + i) * 8 + file - i));
            }
        }

        return north_east_ray;
    }

    U64 getEastRay(int ind) {
        int rank = rankFromBitIndex(ind);
        int file = fileFromBitIndex(ind);

        U64 east_ray = 0ULL;

        for (int i = file - 1; i >= 0; i--) {
            east_ray |= (1ULL << (rank * 8 + i));
        }

        return east_ray;
    }

    U64 getSouthEastRay(int ind) {
        int rank = rankFromBitIndex(ind);
        int file = fileFromBitIndex(ind);

        U64 south_east_ray = 0ULL;

        for (int i = 1; i < 8; i++) {
            if (rank - i >= 0 && file - i >= 0) {
                south_east_ray |= (1ULL << ((rank - i) * 8 + file - i));
            }
        }

        return south_east_ray;
    }

    U64 getSouthRay(int ind) {
        int rank = rankFromBitIndex(ind);
        int file = fileFromBitIndex(ind);

        U64 south_ray = 0ULL;

        for (int i = rank - 1; i >= 0; i--) {
            south_ray |= (1ULL << (i * 8 + file));
        }

        return south_ray;
    }

    U64 getSouthWestRay(int ind) {
        int rank = rankFromBitIndex(ind);
        int file = fileFromBitIndex(ind);

        U64 south_west_ray = 0ULL;

        for (int i = 1; i < 8; i++) {
            if (rank - i >= 0 && file + i < 8) {
                south_west_ray |= (1ULL << ((rank - i) * 8 + file + i));
            }
        }

        return south_west_ray;
    }

    U64 getWestRay(int ind) {
        int rank = rankFromBitIndex(ind);
        int file = fileFromBitIndex(ind);

        U64 west_ray = 0ULL;

        for (int i = file + 1; i < 8; i++) {
            west_ray |= (1ULL << (rank * 8 + i));
        }

        return west_ray;
    }

    U64 getNorthWestRay(int ind) {
        int rank = rankFromBitIndex(ind);
        int file = fileFromBitIndex(ind);

        U64 north_west_ray = 0ULL;

        for (int i = 1; i < 8; i++) {
            if (rank + i < 8 && file + i < 8) {
                north_west_ray |= (1ULL << ((rank + i) * 8 + file + i));
            }
        }

        return north_west_ray;
    }

    std::vector<U64> getStraightRays(int ind) {
        std::vector<U64> rays;

        rays.push_back(getNorthRay(ind));
        rays.push_back(getEastRay(ind));
        rays.push_back(getSouthRay(ind));
        rays.push_back(getWestRay(ind));

        return rays;
    }

    std::vector<U64> getDiagonalRays(int ind) {
        std::vector<U64> rays;

        rays.push_back(getNorthEastRay(ind));
        rays.push_back(getSouthEastRay(ind));
        rays.push_back(getSouthWestRay(ind));
        rays.push_back(getNorthWestRay(ind));

        return rays;
    }

    std::vector<std::vector<U64>> getAllStraightRayBitMasks() {
        std::vector<std::vector<U64>> allStraightRayBitMasks;

        for (int i = 0; i < 64; i++) {
            allStraightRayBitMasks.push_back(getStraightRays(i));
        }

        return allStraightRayBitMasks;
    }

    std::vector<std::vector<U64>> getAllDiagonalRayBitMasks() {
        std::vector<std::vector<U64>> allDiagonalRayBitMasks;

        for (int i = 0; i < 64; i++) {
            allDiagonalRayBitMasks.push_back(getDiagonalRays(i));
        }

        return allDiagonalRayBitMasks;
    }
}