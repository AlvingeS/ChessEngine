#pragma once

#include "ChessEngine/src/common.h"

namespace logic {
namespace bitmasks {

bitmask applyKnightMoves(int ind);
bitmask removeWrapAroundKnightMoves(bitmask knightMoves, int ind);
bitmask getKnightBitmask(int ind);
const std::vector<bitmask> getAllKnightBitmasks();

} // namespace bitmasks
} // namespace logic