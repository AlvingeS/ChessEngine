#pragma once

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
}