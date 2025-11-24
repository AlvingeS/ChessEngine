#pragma once

#include "logic/attack_tables/attack_tables.h"

namespace model {
    class Movelist;
    class Board;
    class Bitboards;
    class OccupancyMasks;
}

namespace logic {

class RookGen {

public:
    RookGen(model::Board& board);
    
    void generate(
        bool is_w,
        model::Movelist& movelist
    );

private:
    const model::Bitboards& bbs_;
    const model::OccupancyMasks& occupancy_masks_;

    ray_attack_table& line_ray_attack_table_;
};

} // namespace logic