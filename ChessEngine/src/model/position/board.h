#pragma once

#include "model/position/bitboards.h"
#include "model/position/piece_map.h"
#include "model/position/state_bitmasks.h"
#include "model/position/z_hasher.h"

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