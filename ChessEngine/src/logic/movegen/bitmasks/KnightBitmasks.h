#pragma once

#include "ChessEngine/src/common.h"

namespace logic {
    
bitmask getKnightBitmask(int ind);
const std::vector<bitmask> getAllKnightBitmasks();

bitmask applyKnightMoves(int ind);
bitmask removeWrapAroundKnightMoves(bitmask knightMoves, int ind);

} // namespace logic