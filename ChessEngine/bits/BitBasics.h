#pragma once

namespace bits {
    typedef unsigned long long U64;

    inline bool getBit(U64 a, int i) {
        return (a >> i) & 1;
    }

    inline int indexOfLSB(U64 a) {
        if (a == 0) {
            return 0;
        }

        return __builtin_ctzll(a);
    }

    inline int indexOfMSB(U64 a) {
        return 63 - __builtin_clzll(a);
    }
}