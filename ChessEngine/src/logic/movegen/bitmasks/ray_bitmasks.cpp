#include "logic/movegen/bitmasks/ray_bitmasks.h"

#include "logic/movegen/utils/chess_utils.h"

namespace logic {

bitmask RayBitmasks::get_n_ray(int ind) 
{
    int rank = ChessUtils::rank_from_bit_index(ind);
    int file = ChessUtils::file_from_bit_index(ind);

    bitmask north_ray = 0ULL;

    for (int i = rank + 1; i < 8; i++) {
        north_ray |= (1ULL << (i * 8 + file));
    }

    return north_ray;
}

bitmask RayBitmasks::get_ne_ray(int ind) 
{
    int rank = ChessUtils::rank_from_bit_index(ind);
    int file = ChessUtils::file_from_bit_index(ind);

    bitmask north_east_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank + i < 8 && file - i >= 0) {
            north_east_ray |= (1ULL << ((rank + i) * 8 + file - i));
        }
    }

    return north_east_ray;
}

bitmask RayBitmasks::get_e_ray(int ind) 
{
    int rank = ChessUtils::rank_from_bit_index(ind);
    int file = ChessUtils::file_from_bit_index(ind);

    bitmask east_ray = 0ULL;

    for (int i = file - 1; i >= 0; i--) {
        east_ray |= (1ULL << (rank * 8 + i));
    }

    return east_ray;
}

bitmask RayBitmasks::get_se_ray(int ind) 
{
    int rank = ChessUtils::rank_from_bit_index(ind);
    int file = ChessUtils::file_from_bit_index(ind);

    bitmask south_east_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank - i >= 0 && file - i >= 0) {
            south_east_ray |= (1ULL << ((rank - i) * 8 + file - i));
        }
    }

    return south_east_ray;
}

bitmask RayBitmasks::get_s_ray(int ind) 
{
    int rank = ChessUtils::rank_from_bit_index(ind);
    int file = ChessUtils::file_from_bit_index(ind);

    bitmask south_ray = 0ULL;

    for (int i = rank - 1; i >= 0; i--) {
        south_ray |= (1ULL << (i * 8 + file));
    }

    return south_ray;
}

bitmask RayBitmasks::get_sw_ray(int ind) 
{
    int rank = ChessUtils::rank_from_bit_index(ind);
    int file = ChessUtils::file_from_bit_index(ind);

    bitmask south_west_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank - i >= 0 && file + i < 8) {
            south_west_ray |= (1ULL << ((rank - i) * 8 + file + i));
        }
    }

    return south_west_ray;
}

bitmask RayBitmasks::get_w_ray(int ind) 
{
    int rank = ChessUtils::rank_from_bit_index(ind);
    int file = ChessUtils::file_from_bit_index(ind);

    bitmask west_ray = 0ULL;

    for (int i = file + 1; i < 8; i++) {
        west_ray |= (1ULL << (rank * 8 + i));
    }

    return west_ray;
}

bitmask RayBitmasks::get_nw_ray(int ind) 
{
    int rank = ChessUtils::rank_from_bit_index(ind);
    int file = ChessUtils::file_from_bit_index(ind);

    bitmask north_west_ray = 0ULL;

    for (int i = 1; i < 8; i++) {
        if (rank + i < 8 && file + i < 8) {
            north_west_ray |= (1ULL << ((rank + i) * 8 + file + i));
        }
    }

    return north_west_ray;
}

RayBitmasks::StraightRays RayBitmasks::get_straight_rays(int ind) 
{
    return {
        get_n_ray(ind),
        get_e_ray(ind),
        get_s_ray(ind),
        get_w_ray(ind)
    };
}

RayBitmasks::DiagonalRays RayBitmasks::get_diagonal_rays(int ind) 
{
    return {
        get_ne_ray(ind),
        get_se_ray(ind),
        get_sw_ray(ind),
        get_nw_ray(ind)
    };
}
std::vector<RayBitmasks::StraightRays> RayBitmasks::get_all_straight_ray_bitmasks() 
{
    std::vector<RayBitmasks::StraightRays> allStraightRayBitmasks;

    for (int i = 0; i < 64; i++) {
        allStraightRayBitmasks.push_back(get_straight_rays(i));
    }

    return allStraightRayBitmasks;
}

std::vector<RayBitmasks::DiagonalRays> RayBitmasks::get_all_diagonal_ray_bitmasks() 
{
    std::vector<RayBitmasks::DiagonalRays> allDiagonalRayBitmasks;

    for (int i = 0; i < 64; i++) {
        allDiagonalRayBitmasks.push_back(get_diagonal_rays(i));
    }

    return allDiagonalRayBitmasks;
}

} // namespace logic