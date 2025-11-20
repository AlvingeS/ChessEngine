#pragma once

#include "model/types.h"

#include <vector>

namespace model {
    class Bitboards;
    class OccupancyMasks;
    class Board;
    class Movelist;
}

namespace logic {

class KingGen {

public:
    KingGen(model::Board& board);
    void generate(bool is_w, model::Movelist& movelist);

private:
    const model::Bitboards& bitboards_;
    const model::OccupancyMasks& occupancy_masks_;
    
    attack_table& king_attack_table_;
};

} // namespace logic