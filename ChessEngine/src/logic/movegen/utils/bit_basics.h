#pragma once

#include "model/types.h"

#include <vector>

namespace logic {

struct BitBasics {

    static bool get_bit(bitmask mask, int i);
    static int lsb_idx(bitmask mask); 
    static int msb_idx(bitmask mask);
    static void get_bit_idxs(std::vector<int>& idxs, bitboard bb);
    
    static int pop_count(bitmask mask);
    static void print_bitmask_in_bits(bitmask mask);
    
};

} // namespace logic

#include "logic/movegen/utils/bit_basics.inl"