#pragma once

#include "model/types.h"
#include "model/position/piece_type.h"

namespace logic::chess_utils {
    
bitmask get_file_mask(int file);
bitmask get_rank_mask(int rank);
int rank_from_sq(sq_idx sq);
int file_from_sq(sq_idx sq);

int abs(int n);
int manhattan_distance(int i, int j);

} // namespace logic::chess_utils

#include "logic/movegen/utils/chess_utils.inl"