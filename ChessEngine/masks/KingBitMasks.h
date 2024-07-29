#pragma once

#include "ChessEngine/common.h"

namespace masks {

U64 applyKingMoves(const int ind);
void removeWrapAroundKingMoves(U64& kingMoves, const int ind);
U64 getKingBitMask(const int ind);
const std::vector<U64> getAllKingBitMasks();

} // namespace masks