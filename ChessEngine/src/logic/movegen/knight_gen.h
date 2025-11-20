#pragma once

#include "model/types.h"
#include <vector>

namespace model {
    class Movelist;
    class Board;
    class Bitboards;
    class OccupancyMasks;
}

namespace logic {

class KnightGen {

public:
    KnightGen(model::Board& board);
    void generate(bool is_w, model::Movelist& movelist);

private:
    const model::Bitboards& bitboards_;
    const model::OccupancyMasks& occupancy_masks_;
    
    attack_table& knight_attack_table_;
};

} // namespace logic