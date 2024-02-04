#include <iostream>
#include <bitset>
#include "ChessUtils.h"

namespace bits {
    void printU64InBits(unsigned long long int n) {
        std::cout << std::bitset<64>(n) << std::endl;
    }

    void getBitIndices(std::vector<int>& indices, U64 bitboard) {
        indices.clear();

        while (bitboard != 0) {
            int index = __builtin_ctzll(bitboard);

            indices.push_back(index);

            bitboard &= (bitboard - 1);
        }
    }
}
