#include "BitMasks.h"
#include "BitDir.h"

namespace bits {
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
}