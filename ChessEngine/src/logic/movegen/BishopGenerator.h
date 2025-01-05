#pragma once

#include "ChessEngine/src/logic/movegen/RayLogic.h"
#include "ChessEngine/src/logic/movegen/bitmasks/RayBitmasks.h"

#include "ChessEngine/src/model/move/Move.h"
#include "ChessEngine/src/model/move/Movelist.h"
#include "ChessEngine/src/model/board/Bitboards.h"
#include "ChessEngine/src/model/board/StateBitmasks.h"

namespace logic {

class BishopGenerator {

public:
    // Constructor
    BishopGenerator(const model::Bitboards& bitboards, const model::StateBitmasks& stateBitmasks);
    
    // Public member functions
    void generate(
        bool isWhite, 
        model::Movelist& movelist
    );

private:
    // Private member functions
    const model::Bitboards& _bitboardsRef;
    const model::StateBitmasks& _stateBitmasksRef;

    // Memory
    std::vector<DiagonalRays> _diagonalRayBitmasks;
};

} // namespace logic