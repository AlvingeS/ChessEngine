#pragma once

#include "model/types.h"
#include "model/position/bitboards.h"
#include "model/position/piece_map.h"
#include "model/position/occupancy_masks.h"

namespace model
{
struct Position {
    Bitboards bbs;
    PieceMap piece_map;
    OccupancyMasks occ_masks;
    bitmask ep_target_mask;
    castle_rights c_rights;
    bool is_w;
    int halfmove_clock;

    Position()
        : bbs(Bitboards())
        , piece_map(bbs)
        , occ_masks(bbs)
        , ep_target_mask{0ULL}
        , c_rights{0b1111}
        , is_w{true}
        , halfmove_clock{0}
    {}
};

}