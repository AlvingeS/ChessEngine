#pragma once

#include "logic/movegen/bitmasks/ray_bitmasks.h"

namespace model {
    class Movelist;
    class Board;
    class Bitboards;
    class StateBitmasks;
}

namespace logic {

class QueenGenerator {

public:
    QueenGenerator(model::Board& board);
    
    void generate(
        bool is_w, 
        model::Movelist& movelist
    );

private:
    const model::Bitboards& bitboards_;
    const model::StateBitmasks& state_bitmasks_;

    std::vector<RayBitmasks::StraightRays> line_ray_masks_;
    std::vector<RayBitmasks::DiagonalRays> diag_ray_masks;
};

} // namespace logic