#pragma once

#include "ChessEngine/src/representation/move/Movelist.h"
#include "ChessEngine/src/logic/movegen/RayLogic.h"

#include "ChessEngine/src/representation/board/Bitboards.h"
#include "ChessEngine/src/representation/board/GameStateBitmasks.h"

#include "ChessEngine/src/representation/move/Move.h"
#include "ChessEngine/src/logic/movegen/bitmasks/PawnBitmasks.h"

// FIXME: We cant have a dependency to perft
#include "ChessEngine/src/engine/search/SearchMemory.h"


namespace logic {
namespace movegen {

class PawnGenerator {

public:
    // Constructor
    PawnGenerator(
        const representation::board::Bitboards& bitboards,
        const representation::board::GameStateBitmasks& gameStateBitmasks
    );
    
    // Public member functions
    void generate(
        bool isWhite,
        representation::move::Movelist& moveListRef,
        int currentDepth,
        engine::search::SearchMemory& searchMemory
    );

private:
    // Private member functions
    const representation::board::Bitboards& _bitboardsRef;
    const representation::board::GameStateBitmasks& _gameStateBitmasksRef;

    // Memory
    std::vector<bitmask> _whitePawnStraightMoveBitmasks;
    std::vector<bitmask> _blackPawnStraightMoveBitmasks;
    std::vector<bitmask> _whitePawnCaptureMoveBitmasks;
    std::vector<bitmask> _blackPawnCaptureMoveBitmasks;
};

} // namespace movegen
} // namespace logic