#pragma once

#include "model/position/bitboards.h"
#include "model/position/piece_map.h"
#include "model/position/occupancy_masks.h"
#include "model/position/z_hasher.h"

namespace model {

struct Board {
    Bitboards bbs;
    PieceMap piece_map;
    OccupancyMasks occupancy_masks;
    ZHasher z_hasher;

    Board() 
        : piece_map(bbs)
        , occupancy_masks(bbs)
        , z_hasher(piece_map) 
    {}
};

} // namespace model