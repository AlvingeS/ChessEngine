#pragma once

#include "model/types.h"
#include "model/position/piece_type.h"

namespace logic {

struct UndoInfo {
    model::Piece::Type captured_piece_type;
    model::Piece::Type moved_piece_type;
    castle_rights c_rights;
    bitmask ep_target_mask;
    uint64_t z_hash;

    UndoInfo() {
        captured_piece_type = model::Piece::Type::EMPTY;
        moved_piece_type    = model::Piece::Type::EMPTY;
        c_rights = 0b1111;
        ep_target_mask = 0ULL;
        z_hash = 0ULL;
    }
};
    
} // namespace logic