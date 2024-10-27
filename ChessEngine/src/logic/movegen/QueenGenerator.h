#pragma once

#include "ChessEngine/src/representation/move/Movelist.h"
#include "ChessEngine/src/logic/movegen/RayLogic.h"

#include "ChessEngine/src/representation/board/Bitboards.h"
#include "ChessEngine/src/representation/board/GameStateBitmasks.h"

#include "ChessEngine/src/representation/move/Move.h"
#include "ChessEngine/src/logic/movegen/bitmasks/RayBitmasks.h"

namespace logic {
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
} // namespace logic