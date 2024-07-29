#pragma once

#include "ChessEngine/common.h"

namespace masks {

U64 applyKnightMoves(const int ind);
void removeWrapAroundKnightMoves(U64& knightMoves, const int ind);
U64 getKnightBitMask(const int ind);
const std::vector<U64> getAllKnightBitMasks();

}