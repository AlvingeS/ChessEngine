#pragma once
#include "logic/movegen/bitmasks/ray_bitmasks.h"

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
    std::vector<RayBitmasks::DiagonalRays> diag_ray_masks;
};

} // namespace logic