#pragma once

#include "ChessEngine/src/common.h"

namespace logic {

struct KnightBitmasks {

    static bitmask getKnightBitmask(int ind);
    static const std::vector<bitmask> getAllKnightBitmasks();
    
    static bitmask applyKnightMoves(int ind);
    static bitmask removeWrapAroundKnightMoves(bitmask knightMoves, int ind);

};

} // namespace logic