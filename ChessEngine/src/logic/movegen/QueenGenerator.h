#pragma once

#include "ChessEngine/src/model/move/Movelist.h"
#include "ChessEngine/src/logic/movegen/RayLogic.h"

#include "ChessEngine/src/model/board/Board.h"

#include "ChessEngine/src/model/move/Move.h"
#include "ChessEngine/src/logic/movegen/bitmasks/RayBitmasks.h"

namespace logic {

class QueenGenerator {

public:
    // Constructor
    QueenGenerator(model::Board& board);
    
    // Public member functions
    void generate(
        bool isWhite, 
        model::Movelist& moveList
    );

private:
    // Private member functions
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;

    // Memory
    std::vector<StraightRays> _straightRayBitmasks;
    std::vector<DiagonalRays> _diagonalRayBitmasks;
};

} // namespace logic