#pragma once

#include "model/types.h"
#include <vector>

namespace model {
    class Board;
    class Movelist;
    class Bitboards;
    class StateBitmasks;
}

namespace logic {

class PawnGenerator {

public:
    PawnGenerator(model::Board& board);
    
    void generate(
        bool is_w,
        model::Movelist& movelist,
        bitmask ep_target_mask
    );

private:
    const model::Bitboards& bitboards_;
    const model::StateBitmasks& state_bitmasks_;

    std::vector<bitmask> w_pawn_straight_move_masks_;
    std::vector<bitmask> b_pawn_straight_move_masks_;
    std::vector<bitmask> w_pawn_capture_move_masks;
    std::vector<bitmask> b_pawn_capture_move_masks;
};

} // namespace logic