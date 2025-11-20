#pragma once

#include <cstdint>

typedef uint64_t bitboard;
typedef uint64_t bitmask;
typedef bitmask attack_table[64];
typedef bitmask ray_attack_table[64][4];

enum LineDir : int {N = 0, E = 1, S = 2, W = 3};
enum DiagDir : int {NE = 0, SE = 1, SW = 2, NW = 3};