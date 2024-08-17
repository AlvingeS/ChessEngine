#pragma once

#include "ChessEngine/common.h"

namespace masks {

bitmask applyKingMoves(const int ind);
void removeWrapAroundKingMoves(bitmask& kingMoves, const int ind);
bitmask getKingBitmask(const int ind);
const std::vector<bitmask> getAllKingBitmasks();

} // namespace masks