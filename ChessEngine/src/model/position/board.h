#pragma once

#include "model/position/bitboards.h"
#include "model/position/piece_map.h"
#include "model/position/state_bitmasks.h"
#include "model/position/z_hasher.h"

namespace model {

struct Board {
    Bitboards bitboards;
    PieceMap piece_map;
    StateBitmasks state_bitmasks;
    ZHasher z_hasher;

    Board() 
        : piece_map(bitboards)
        , state_bitmasks(bitboards)
        , z_hasher(piece_map) 
    {}
};

} // namespace model