
#include "BitBasics.h"
#include <vector>

namespace bits {
    U64 getFileMask(int file);
    U64 getRankMask(int rank);
    std::vector<std::vector<U64>> create_straight_ray_bitmasks();
    std::vector<std::vector<U64>> create_diagonal_ray_bitmasks();
    std::vector<std::vector<U64>> create_knight_ray_bitmasks();
    std::vector<std::vector<U64>> create_king_ray_bitmasks();
    std::vector<std::vector<U64>> create_pawn_ray_bitmasks();
}