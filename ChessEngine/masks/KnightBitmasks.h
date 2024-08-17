#pragma once

#include "ChessEngine/common.h"

namespace masks {

bitmask applyKnightMoves(int ind);
bitmask removeWrapAroundKnightMoves(bitmask knightMoves, int ind);
bitmask getKnightBitmask(int ind);
const std::vector<bitmask> getAllKnightBitmasks();

}