#pragma once

namespace bits {
    typedef unsigned long long U64;

    inline U64 bitwiseAnd(U64 a, U64 b) {
        return a & b;
    }

    inline U64 bitwiseOr(U64 a, U64 b) {
        return a | b;
    }

    inline U64 bitwiseXor(U64 a, U64 b) {
        return a ^ b;
    }

    inline U64 bitwiseNot(U64 a) {
        return ~a;
    }

    inline bool getBit(U64 a, int i) {
        return (a >> i) & 1;
    }

    inline int rankFromBitIndex(int i) {
        return i / 8;
    }

    inline int fileFromBitIndex(int i) {
        return 7 - i % 8;
    }
}