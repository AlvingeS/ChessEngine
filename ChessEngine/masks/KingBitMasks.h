#pragma once

#include "ChessEngine/common.h"

namespace masks {
    U64 applyKingMoves(int ind);
    void removeWrapAroundKingMoves(U64& kingMoves, int ind);
    U64 getKingBitMask(int ind);
    std::vector<U64> getAllKingBitMasks();
}