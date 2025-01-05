#pragma once

#include "ChessEngine/src/logic/movegen/RayLogic.h"
#include "ChessEngine/src/logic/movegen/bitmasks/RayBitmasks.h"

#include "ChessEngine/src/model/move/Move.h"
#include "ChessEngine/src/model/move/Movelist.h"
#include "ChessEngine/src/model/board/Board.h"

namespace logic {

class BishopGenerator {

public:
    // Constructor
    BishopGenerator(model::Board& board);
    
    // Public member functions
    void generate(
        bool isWhite, 
        model::Movelist& movelist
    );

private:
    // Private member functions
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;

    // Memory
    std::vector<DiagonalRays> _diagonalRayBitmasks;
};

} // namespace logic