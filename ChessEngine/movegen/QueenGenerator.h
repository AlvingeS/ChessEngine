#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/board/Bitboards.h"
#include "ChessEngine/board/GameStateBitmasks.h"
#include "ChessEngine/move/Move.h"
#include "ChessEngine/masks/RayBitmasks.h"
#include "ChessEngine/movegen/Movelist.h"

#include "ChessEngine/movegen/RayLogic.h"

namespace movegen {

class QueenGenerator {

public:
    // Constructor
    QueenGenerator(const board::Bitboards& bitboards, const board::GameStateBitmasks& gameStateBitmasks);
    
    // Public member functions
    void generate(
        bool isWhite, 
        Movelist& moveListRef
    );

private:
    // Private member functions
    const board::Bitboards& _bitboardsRef;
    const board::GameStateBitmasks& _gameStateBitmasksRef;

    // Memory
    std::vector<masks::StraightRays> _straightRayBitmasks;
    std::vector<masks::DiagonalRays> _diagonalRayBitmasks;
};

} // namespace movegen