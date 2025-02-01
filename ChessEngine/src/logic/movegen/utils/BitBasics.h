#pragma once

#include "ChessEngine/src/common.h"

namespace logic {

bool getBit(bitmask a, int i);
int indexOfLSB(bitmask a); 
int indexOfMSB(bitmask a);
int popCount(bitmask a);

void getBitIndices(std::vector<int>& indices, bitmask bitboard);
void printbitmaskInBits(bitmask n);

} // namespace logic

#include "ChessEngine/src/logic/movegen/utils/BitBasics.inl"