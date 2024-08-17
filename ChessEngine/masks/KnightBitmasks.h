#pragma once

#include "ChessEngine/common.h"

namespace masks {

bitmask applyKnightMoves(const int ind);
void removeWrapAroundKnightMoves(bitmask& knightMoves, const int ind);
bitmask getKnightBitmask(const int ind);
const std::vector<bitmask> getAllKnightBitmasks();

}