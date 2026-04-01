#pragma once

#include "model/types.h"

namespace logic {

struct UndoInfo {
    PieceType captured_piece_type;
    PieceType moved_piece_type;
    castle_rights c_rights;
    bitmask ep_target_mask;
    uint64_t z_hash;
    int last_irreversible_move_idx;

    UndoInfo() {
        captured_piece_type = PieceType::EMPTY;
        moved_piece_type    = PieceType::EMPTY;
        c_rights = 0b1111;
        ep_target_mask = 0ULL;
        z_hash = 0ULL;
        last_irreversible_move_idx = 0;
    }
};
    
} // namespace logic