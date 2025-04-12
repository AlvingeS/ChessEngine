#pragma once

#include "ChessEngine/src/common.h"

namespace logic {

struct KingBitmasks {

    static bitmask getKingBitmask(int ind);
    static const std::vector<bitmask> getAllKingBitmasks();
    
    static bitmask applyKingMoves(int ind);
    static bitmask removeWrapAroundKingMoves(bitmask kingMoves, int ind);

};

} // namespace logic