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

    std::vector<StraightRays> _straightRayBitmasks;
    std::vector<DiagonalRays> _diagonalRayBitmasks;
};

} // namespace logic