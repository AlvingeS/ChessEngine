#pragma once

#include "ChessEngine/src/model/move/Movelist.h"
#include "ChessEngine/src/logic/movegen/RayLogic.h"

#include "ChessEngine/src/model/board/Bitboards.h"
#include "ChessEngine/src/model/board/StateBitmasks.h"

#include "ChessEngine/src/model/move/Move.h"
#include "ChessEngine/src/logic/movegen/bitmasks/PawnBitmasks.h"

// FIXME: We cant have a dependency to perft
#include "ChessEngine/src/engine/search/SearchMemory.h"


namespace logic {

class PawnGenerator {

public:
    // Constructor
    PawnGenerator(
        const model::Bitboards& bitboards,
        const model::StateBitmasks& stateBitmasks
    );
    
    // Public member functions
    void generate(
        bool isWhite,
        model::Movelist& moveListRef,
        int currentDepth,
        engine::SearchMemory& searchMemory
    );

private:
    // Private member functions
    const model::Bitboards& _bitboardsRef;
    const model::StateBitmasks& _stateBitmasksRef;

    // Memory
    std::vector<bitmask> _whitePawnStraightMoveBitmasks;
    std::vector<bitmask> _blackPawnStraightMoveBitmasks;
    std::vector<bitmask> _whitePawnCaptureMoveBitmasks;
    std::vector<bitmask> _blackPawnCaptureMoveBitmasks;
};

} // namespace logic