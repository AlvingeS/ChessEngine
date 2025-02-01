#pragma once

#include "ChessEngine/src/common.h"

namespace logic {

bitmask getKingBitmask(int ind);
const std::vector<bitmask> getAllKingBitmasks();

bitmask applyKingMoves(int ind);
bitmask removeWrapAroundKingMoves(bitmask kingMoves, int ind);

} // namespace logic