#pragma once

#include <cstdint>

typedef uint64_t bitboard;
typedef uint64_t bitmask;
typedef bitmask attack_table[64];
typedef bitmask ray_attack_table[64][8];
typedef int sq_t;
typedef unsigned char castle_rights;

// enum LineDir : int {N = 0, E = 1, S = 2, W = 3};
// enum DiagDir : int {NE = 0, SE = 1, SW = 2, NW = 3};

enum Direction {
    N     = 0,
    NE    = 1,
    E     = 2,
    SE    = 3,
    S     = 4,
    SW    = 5,
    W     = 6,
    NW    = 7,
    COUNT = 8
};

enum class LoopControl {
    Continue,
    Break
};