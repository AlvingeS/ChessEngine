#pragma once

#include "ChessEngine/src/common.h"

namespace game {
namespace masks {

bitmask applyKingMoves(int ind);
bitmask removeWrapAroundKingMoves(bitmask kingMoves, int ind);
bitmask getKingBitmask(int ind);
const std::vector<bitmask> getAllKingBitmasks();

} // namespace masks
} // namespace game