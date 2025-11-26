#include "logic/movegen/utils/bits.h"

#include <bitset>

namespace logic::bits {

void get_bit_idxs(std::vector<int>& idxs, bitboard bb)
{ 
    idxs.clear();

    while (bb != 0) {
        int idx = __builtin_ctzll(bb);

        idxs.push_back(idx);

        bb &= (bb - 1);
    }
}

} // namespace logic::bits