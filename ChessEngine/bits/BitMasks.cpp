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

    std::vector<std::vector<U64>> create_straight_ray_bitmasks() {
        std::vector<std::vector<U64>> rays_bitmasks = std::vector<std::vector<U64>>(64);
        std::vector<U64> rays = std::vector<U64>(4);
        
        for (int ind = 0; ind < 64; ind++) {
            U64 north_ray = 0ULL;
            U64 south_ray = 0ULL;
            U64 east_ray = 0ULL;
            U64 west_ray = 0ULL;

            int rank = rankFromBitIndex(ind);
            int file = fileFromBitIndex(ind);

            for (int i = rank + 1; i < 8; i++) {
                north_ray |= (1ULL << (i * 8 + file));
            }

            for (int i = rank - 1; i >= 0; i--) {
                south_ray |= (1ULL << (i * 8 + file));
            }

            for (int i = file + 1; i < 8; i++) {
                east_ray |= (1ULL << (rank * 8 + i));
            }

            for (int i = file - 1; i >= 0; i--) {
                west_ray |= (1ULL << (rank * 8 + i));
            }

            rays[0] = north_ray;
            rays[1] = east_ray;
            rays[2] = south_ray;
            rays[3] = west_ray;

            rays_bitmasks[ind] = rays;
        }

        return rays_bitmasks;
    }

    std::vector<std::vector<U64>> create_diagonal_ray_bitmasks() {
        std::vector<std::vector<U64>> rays_bitmasks = std::vector<std::vector<U64>>(64);
        std::vector<U64> rays = std::vector<U64>(4);

        for (int ind = 0; ind < 64; ind++) {
            U64 north_east_ray = 0ULL;
            U64 south_east_ray = 0ULL;
            U64 south_west_ray = 0ULL;
            U64 north_west_ray = 0ULL;

            int rank = rankFromBitIndex(ind);
            int file = fileFromBitIndex(ind);

            for (int i = 1; i < 8; i++) {
                if (rank + i < 8 && file + i < 8) {
                    north_east_ray |= (1ULL << ((rank + i) * 8 + file + i));
                }

                if (rank - i >= 0 && file + i < 8) {
                    south_east_ray |= (1ULL << ((rank - i) * 8 + file + i));
                }

                if (rank - i >= 0 && file - i >= 0) {
                    south_west_ray |= (1ULL << ((rank - i) * 8 + file - i));
                }

                if (rank + i < 8 && file - i >= 0) {
                    north_west_ray |= (1ULL << ((rank + i) * 8 + file - i));
                }
            }

            rays[0] = north_east_ray;
            rays[1] = south_east_ray;
            rays[2] = south_west_ray;
            rays[3] = north_west_ray;

            rays_bitmasks[ind] = rays;
        }

        return rays_bitmasks;
    }


}