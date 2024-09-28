#pragma once

#include "ChessEngine/src/game/movegen/Movelist.h"
#include "ChessEngine/src/game/movegen/RayLogic.h"

#include "ChessEngine/src/game/board/Bitboards.h"
#include "ChessEngine/src/game/board/GameStateBitmasks.h"
#include "ChessEngine/src/game/move/Move.h"
#include "ChessEngine/src/game/masks/PawnBitmasks.h"

// FIXME: We cant have a dependency to perft
#include "ChessEngine/src/ponder/perft/SearchMemory.h"

namespace game {
namespace movegen {

class PawnGenerator {

public:
    // Constructor
    PawnGenerator(
        const board::Bitboards& bitboards,
        const board::GameStateBitmasks& gameStateBitmasks
    );
    
    // Public member functions
    void generate(
        bool isWhite,
        Movelist& moveListRef,
        int currentDepth,
        ponder::perft::SearchMemory& searchMemory
    );

private:
    // Private member functions
    const board::Bitboards& _bitboardsRef;
    const board::GameStateBitmasks& _gameStateBitmasksRef;

    // Memory
    std::vector<bitmask> _whitePawnStraightMoveBitmasks;
    std::vector<bitmask> _blackPawnStraightMoveBitmasks;
    std::vector<bitmask> _whitePawnCaptureMoveBitmasks;
    std::vector<bitmask> _blackPawnCaptureMoveBitmasks;
};

} // namespace movegen
} // namespace game