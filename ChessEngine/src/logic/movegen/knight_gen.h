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

class KnightGenerator {

public:
    KnightGenerator(model::Board& board);
    void generate(bool is_w, model::Movelist& movelist);

private:
    const model::Bitboards& bitboards_;
    const model::OccupancyMasks& state_bitmasks_;
    
    attack_table& knight_attack_table_;
};

} // namespace logic