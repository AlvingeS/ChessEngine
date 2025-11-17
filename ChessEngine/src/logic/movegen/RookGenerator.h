#pragma once

#include "logic/movegen/bitmasks/RayBitmasks.h"

namespace model {
    class Movelist;
    class Board;
    class Bitboards;
    class StateBitmasks;
}

namespace logic {

class RookGenerator {

public:
    RookGenerator(model::Board& board);
    
    void generate(
        bool isWhite,
        model::Movelist& moveList
    );

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;

    std::vector<RayBitmasks::StraightRays> _straightRayBitmasks;
};

} // namespace logic