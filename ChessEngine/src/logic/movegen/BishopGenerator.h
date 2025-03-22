#pragma once
#include "ChessEngine/src/common.h"
#include "ChessEngine/src/logic/movegen/bitmasks/RayBitmasks.h"

namespace model {
    class Movelist;
    class Board;
    class Bitboards;
    class StateBitmasks;
}

namespace logic {

class BishopGenerator {

public:
    BishopGenerator(model::Board& board);
    
    void generate(
        bool isWhite, 
        model::Movelist& movelist
    );

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;
    std::vector<RayBitmasks::DiagonalRays> _diagonalRayBitmasks;
};

} // namespace logic