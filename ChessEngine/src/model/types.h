#pragma once

#include <cstdint>

typedef uint64_t bitboard;
typedef uint64_t bitmask;
typedef bitmask attack_table[64];
typedef bitmask ray_attack_table[64][8];
typedef int sq_t;
typedef unsigned char castle_rights;
typedef int16_t eval_t;

enum PieceType {
    W_PAWN   = 0,
    W_KNIGHT = 1,
    W_BISHOP = 2,
    W_ROOK   = 3,
    W_QUEEN  = 4,
    W_KING   = 5,
    B_PAWN   = 6,
    B_KNIGHT = 7,
    B_BISHOP = 8,
    B_ROOK   = 9,
    B_QUEEN  = 10,
    B_KING   = 11,
    EMPTY    = 12,
};

inline bool is_white(PieceType piece_type) { return piece_type <= W_KING; }

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