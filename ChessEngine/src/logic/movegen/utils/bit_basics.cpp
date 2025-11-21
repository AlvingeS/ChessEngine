#include "logic/movegen/utils/bit_basics.h"

#include <bitset>

namespace logic {

void BitBasics::get_bit_idxs(std::vector<int>& idxs, bitboard bb)
{ 
    idxs.clear();

    while (bb != 0) {
        int idx = __builtin_ctzll(bb);

        idxs.push_back(idx);

        bb &= (bb - 1);
    }
}

} // namespace logic