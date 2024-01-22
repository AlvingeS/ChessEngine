#pragma once

namespace bits {
    typedef unsigned long long U64;

    inline bool getBit(U64 a, int i) {
        return (a >> i) & 1;
    }

    inline int rankFromBitIndex(int i) {
        return i / 8;
    }

    inline int fileFromBitIndex(int i) {
        return i % 8;
    }
}