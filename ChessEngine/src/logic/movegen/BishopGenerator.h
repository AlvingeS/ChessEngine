#pragma once

#include "ChessEngine/src/logic/movegen/RayLogic.h"
#include "ChessEngine/src/logic/movegen/bitmasks/RayBitmasks.h"

#include "ChessEngine/src/representation/move/Move.h"
#include "ChessEngine/src/representation/move/Movelist.h"
#include "ChessEngine/src/representation/board/Bitboards.h"
#include "ChessEngine/src/representation/board/GameStateBitmasks.h"


namespace logic {
namespace movegen {

class BishopGenerator {

public:
    // Constructor
    BishopGenerator(const board::Bitboards& bitboards, const board::GameStateBitmasks& gameStateBitmasks);
    
    // Public member functions
    void generate(
        bool isWhite, 
        Movelist& movelist
    );

private:
    // Private member functions
    const board::Bitboards& _bitboardsRef;
    const board::GameStateBitmasks& _gameStateBitmasksRef;

    // Memory
    std::vector<masks::DiagonalRays> _diagonalRayBitmasks;
};

} // namespace movegen
} // namespace logic