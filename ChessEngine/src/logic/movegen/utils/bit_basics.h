#pragma once

#include "model/types.h"

#include <vector>

namespace logic {

struct BitBasics {

    static bool get_bit(bitmask a, int i);
    static int lsb_index(bitmask a); 
    static int msb_index(bitmask a);
    static void get_bit_indices(std::vector<int>& indices, bitboard bb);
    
    static int pop_count(bitmask a);
    static void print_bitmask_in_bits(bitmask n);
    
};

} // namespace logic

#include "logic/movegen/utils/bit_basics.inl"