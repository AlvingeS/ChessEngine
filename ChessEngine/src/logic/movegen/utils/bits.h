#pragma once

#include "model/types.h"

#include <vector>

namespace logic::bits {

bool get_bit(bitmask mask, int i);
int lsb_idx(bitmask mask); 
int msb_idx(bitmask mask);
void get_bit_idxs(std::vector<int>& idxs, bitboard bb);

int pop_count(bitmask mask);
void print_bitmask_in_bits(bitmask mask);    

} // namespace logic::bits

#include "logic/movegen/utils/bit_basics.inl"