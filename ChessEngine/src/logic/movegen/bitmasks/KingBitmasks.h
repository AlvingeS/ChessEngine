#pragma once

#include "ChessEngine/src/common.h"

namespace logic {
namespace movegen {
namespace bitmasks {

bitmask applyKingMoves(int ind);
bitmask removeWrapAroundKingMoves(bitmask kingMoves, int ind);
bitmask getKingBitmask(int ind);
const std::vector<bitmask> getAllKingBitmasks();

} // namespace bitmasks
} // namespace movegen
} // namespace logic