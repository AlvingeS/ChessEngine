#pragma once

#include "ChessEngine/src/common.h"

namespace logic {
namespace movegen {
namespace utils {

inline bool getBit(bitmask a, int i) 
{
    return (a >> i) & 1;
}

inline int indexOfLSB(bitmask a) 
{
    return __builtin_ctzll(a);
}   

inline int indexOfMSB(bitmask a) 
{
    return 63 - __builtin_clzll(a);
}

inline int popCount(bitmask a) 
{
    return __builtin_popcountll(a);
}

void getBitIndices(std::vector<int>& indices, bitmask bitboard);
void printbitmaskInBits(bitmask n);

} // namespace utils
} // namespace movegen
} // namespace logic