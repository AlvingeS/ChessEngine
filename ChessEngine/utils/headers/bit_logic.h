#pragma once
#include <vector>

namespace utils {
    typedef unsigned long long U64;

    void print_bits(unsigned long long int n);

    inline U64 and_bits(U64 a, U64 b) {
        return a & b;
    }

    inline U64 or_bits(U64 a, U64 b) {
        return a | b;
    }

    inline U64 xor_bits(U64 a, U64 b) {
        return a ^ b;
    }

    inline U64 not_bits(U64 a) {
        return ~a;
    }

    inline bool get_bit(U64 a, int i) {
        return (a >> i) & 1;
    }

    inline int rank_from_bit_index(int i) {
        return i / 8;
    }

    inline int file_from_bit_index(int i) {
        return i % 8;
    }

    std::vector<int> get_bit_indices(U64 bitboard);
}