#pragma once

#include "model/types.h"
#include "model/position/bitboards.h"
#include "model/position/piece_map.h"
#include "model/position/occupancy_masks.h"
#include "model/position/z_hasher.h"

namespace model
{
struct Position {
    Bitboards bbs;
    PieceMap piece_map;
    OccupancyMasks occ_masks;
    ZHasher z_hasher;
    bitmask ep_target_mask;
    bitmask prev_ep_target_mask;
    castle_rights c_rights;

    Position()
        : bbs(Bitboards())
        , piece_map(bbs)
        , occ_masks(bbs)
        , z_hasher(piece_map)
        , ep_target_mask{0ULL}
        , c_rights{0b1111}
    {}
};

}