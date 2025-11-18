#pragma once

#include "model/types.h"

#include <vector>

namespace logic {

struct KnightBitmasks {

    static bitmask get_knight_bitmask(int ind);
    static const std::vector<bitmask> get_all_knight_bitmasks();
    
    static bitmask apply_knight_moves(int ind);
    static bitmask remove_wrap_around_knight_moves(bitmask moves, int ind);

};

} // namespace logic