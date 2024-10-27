#include "ChessEngine/src/logic/movegen/utils/BitBasics.h"

#include <iostream>
#include <bitset>

namespace logic {
namespace movegen {
namespace utils {

void getBitIndices(std::vector<int>& indices, bitboard bitboard)
{ 
    indices.clear();

    while (bitboard != 0) {
        int index = __builtin_ctzll(bitboard);

        indices.push_back(index);

        bitboard &= (bitboard - 1);
    }
}

} // namespace utils
} // namespace movegen
} // namespace logic