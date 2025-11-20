#pragma once

#include "model/types.h"
#include <vector>

namespace model {
    class Board;
    class Movelist;
    class Bitboards;
    class OccupancyMasks;
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
    const model::OccupancyMasks& occupancy_masks_;

    attack_table& w_pawn_quiet_attack_table_;
    attack_table& w_pawn_capture_attack_table_;
    attack_table& b_pawn_quiet_attack_table_;
    attack_table& b_pawn_capture_attack_table_;
};

} // namespace logic