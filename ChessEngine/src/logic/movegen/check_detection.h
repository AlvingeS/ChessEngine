#pragma once

#include "logic/movegen/bitmasks/ray_bitmasks.h"

namespace model {
    class Board;
    class Bitboards;
    class StateBitmasks;
}

namespace logic {

class CheckDetection {

public:
    CheckDetection(model::Board& board);
    bool in_check(bool is_w) const;

private:
    const model::Bitboards& bitboards_;
    const model::StateBitmasks& state_bitmasks_;

    std::vector<RayBitmasks::StraightRays> line_ray_masks_;
    std::vector<RayBitmasks::DiagonalRays> diag_ray_masks;
    std::vector<bitmask> knight_masks_;
    std::vector<bitmask> w_pawn_capture_move_masks;
    std::vector<bitmask> b_pawn_capture_move_masks;
};

} // namespace logic