#pragma once

#include "ChessEngine/common.h"

namespace masks {

bitmask applyKingMoves(int ind);
bitmask removeWrapAroundKingMoves(bitmask kingMoves, int ind);
bitmask getKingBitmask(int ind);
const std::vector<bitmask> getAllKingBitmasks();

} // namespace masks