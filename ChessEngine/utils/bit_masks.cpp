#include "bit_masks.h"
#include "bitdir.h"

namespace utils {
    std::vector<U64> create_straight_ray_bitmasks(int bit_index) {
        std::vector<U64> rays = std::vector<U64>(4);
        
        U64 north_ray = 0ULL;
        U64 south_ray = 0ULL;
        U64 east_ray = 0ULL;
        U64 west_ray = 0ULL;

        int rank = rank_from_bit_index(bit_index);
        int file = file_from_bit_index(bit_index);

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

        return rays;
    }
}