#include "ChessEngine/src/utils/BitBasics.h"

#include <iostream>
#include <bitset>

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

void printBitboardInBits(bitboard n) 
{
    std::cout << std::bitset<64>(n) << std::endl;
}

} // namespace utils