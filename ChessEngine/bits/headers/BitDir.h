#pragma once

namespace bits {
    enum class BitDir {
        N,
        NE,
        E,
        SE,
        S,
        SW,
        W,
        NW
    };

    inline int dir_to_shift(BitDir dir) {
        switch(dir) {
            case BitDir::N: return 8;
            case BitDir::NE: return 7;
            case BitDir::E: return -1;
            case BitDir::SE: return -9;
            case BitDir::S: return -8;
            case BitDir::SW: return -7;
            case BitDir::W: return 1;
            case BitDir::NW: return 9;
            default: return 0;
        }
    }
}