#pragma once

#include "model/position/bitboards.h"
#include "model/position/piece_map.h"
#include "model/position/occupancy_masks.h"
#include "model/position/z_hasher.h"

namespace model {

struct Board {
    Bitboards bitboards;
    PieceMap piece_map;
    OccupancyMasks occupancy_masks;
    ZHasher z_hasher;

    Board() 
        : piece_map(bitboards)
        , occupancy_masks(bitboards)
        , z_hasher(piece_map) 
    {}
};

} // namespace model