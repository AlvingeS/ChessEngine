#pragma once

#include "ChessEngine/common.h"

namespace masks {
    U64 applyKnightMoves(int ind);
    void removeWrapAroundKnightMoves(U64& knightMoves, int ind);
    U64 getKnightBitMask(int ind);
    std::vector<U64> getAllKnightBitMasks();
}