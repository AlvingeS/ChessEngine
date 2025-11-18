#pragma once

#include "model/types.h"
#include "model/position/piece_type.h"

namespace logic {
    
struct ChessUtils {
    
    static bitmask get_file_mask(int file);
    static bitmask get_rank_mask(int rank);
    static int rank_from_bit_index(int i);
    static int file_from_bit_index(int i);
    
    static int abs(int n);
    static int manhattan_distance(int i, int j);

};

} // namespace logic

#include "logic/movegen/utils/chess_utils.inl"