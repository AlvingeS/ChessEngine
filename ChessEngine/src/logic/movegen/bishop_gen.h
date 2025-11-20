#pragma once
#include "logic/attack_tables/attack_tables.h"

namespace model {
    class Movelist;
    class Board;
    class Bitboards;
    class StateBitmasks;
}

namespace logic {

class BishopGen {

public:
    BishopGen(model::Board& board);
    
    void generate(
        bool is_w, 
        model::Movelist& movelist
    );

private:
    const model::Bitboards& bitboards_;
    const model::StateBitmasks& state_bitmasks_;

    ray_attack_table& diag_ray_attack_table_;
};

} // namespace logic