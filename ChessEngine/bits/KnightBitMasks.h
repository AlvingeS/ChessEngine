#pragma once
#include <vector>
#include "BitBasics.h"

namespace bits {
    U64 applyKnightMoves(int ind);
    void removeWrapAroundKnightMoves(U64& knightMoves, int ind);
    U64 getKnightBitMask(int ind);
    std::vector<U64> getAllKnightBitMasks();
}