#pragma once

#include "model/types.h"

#include <vector>

namespace logic {

struct MoveType {
    std::vector<bitmask> diagonal;
    std::vector<bitmask> straight;
};

struct PawnBitmasks {

    static bitmask get_capture_pawn_moves_bitmask(int ind, bool is_w);
    static bitmask get_straight_pawn_moves_bitmask(int ind, bool is_w);
    static std::vector<bitmask> get_all_straight_pawn_move_bitmasks(bool is_w);
    static std::vector<bitmask> get_all_capture_pawn_move_bitmasks(bool is_w);
    
    static bitmask apply_capture_pawn_moves(int ind, bool is_w);
    static bitmask remove_wrap_around_pawn_moves(bitmask moves, int ind);

};

} // namespace logic