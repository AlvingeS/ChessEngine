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
        bool isWhite, 
        model::Movelist& moveList
    );

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;

    std::vector<RayBitmasks::StraightRays> _straightRayBitmasks;
    std::vector<RayBitmasks::DiagonalRays> _diagonalRayBitmasks;
};

} // namespace logic