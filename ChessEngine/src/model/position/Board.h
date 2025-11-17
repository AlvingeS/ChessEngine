#pragma once

#include "model/position/Bitboards.h"
#include "model/position/PieceMap.h"
#include "model/position/StateBitmasks.h"
#include "model/position/ZHasher.h"

namespace model {

struct Board {
    Bitboards bitboards;
    PieceMap pieceMap;
    StateBitmasks stateBitmasks;
    ZHasher zHasher;

    Board() 
        : pieceMap(bitboards)
        , stateBitmasks(bitboards)
        , zHasher(pieceMap) 
    {}
};

} // namespace model