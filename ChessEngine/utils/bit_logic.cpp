#include <iostream>
#include <bitset>
#include "headers/bit_logic.h"

namespace utils {

    void print_bits(unsigned long long int n) {
        std::cout << std::bitset<64>(n) << std::endl;
    }

    std::vector<int> get_bit_indices(U64 bitboard) {
        std::vector<int> indices;

        while (bitboard != 0) {
            int index = __builtin_ctzll(bitboard);

            indices.push_back(index);

            bitboard &= (bitboard - 1);
        }

        return indices;
    }
}
