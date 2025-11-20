#include "logic/movegen/utils/bit_basics.h"

#include <bitset>

namespace logic {

void BitBasics::get_bit_indices(std::vector<int>& indices, bitboard bb)
{ 
    indices.clear();

    while (bb != 0) {
        int idx = __builtin_ctzll(bb);

        indices.push_back(idx);

        bb &= (bb - 1);
    }
}

} // namespace logic