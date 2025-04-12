#pragma once

#include "ChessEngine/src/common.h"

namespace logic {

struct BitBasics {

    static bool getBit(bitmask a, int i);
    static int indexOfLSB(bitmask a); 
    static int indexOfMSB(bitmask a);
    static void getBitIndices(std::vector<int>& indices, bitmask bitboard);
    
    static int popCount(bitmask a);
    static void printBitmaskInBits(bitmask n);
    
};

} // namespace logic

#include "ChessEngine/src/logic/movegen/utils/BitBasics.inl"