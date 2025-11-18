#pragma once

#include "model/types.h"

#include <vector>

namespace logic {

struct KingBitmasks {

    static bitmask get_king_bitmask(int ind);
    static const std::vector<bitmask> get_all_king_bitmasks();
    
    static bitmask apply_king_moves(int ind);
    static bitmask remove_wrap_around_king_moves(bitmask kingMoves, int ind);

};

} // namespace logic