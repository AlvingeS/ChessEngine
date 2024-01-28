#pragma once
#include <vector>
#include "BitBasics.h"

namespace bits {
    U64 applyKingMoves(int ind);
    void removeWrapAroundKingMoves(U64& kingMoves, int ind);
    U64 getKingBitMask(int ind);
    std::vector<U64> getAllKingBitMasks();
}