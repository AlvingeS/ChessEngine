#pragma once

#include "ChessEngine/src/model/move/Movelist.h"
#include "ChessEngine/src/logic/movegen/RayLogic.h"

#include "ChessEngine/src/model/board/Bitboards.h"
#include "ChessEngine/src/model/board/StateBitmasks.h"

#include "ChessEngine/src/model/move/Move.h"
#include "ChessEngine/src/logic/movegen/bitmasks/KnightBitmasks.h"

namespace logic {

class KnightGenerator {

public:
    // Constructor
    KnightGenerator(
        const model::Bitboards& bitboards,
        const model::StateBitmasks& stateBitmasks
    );
    
    // Public member functions
    void generate(bool isWhite, model::Movelist& moveListRef);

private:
    // Private member functions
    const model::Bitboards& _bitboardsRef;
    const model::StateBitmasks& _stateBitmasksRef;

    // Memory
    std::vector<bitmask> _knightBitmasks;
};

} // namespace logic