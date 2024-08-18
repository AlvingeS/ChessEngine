#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/board/Bitboards.h"
#include "ChessEngine/board/GameStateBitmasks.h"
#include "ChessEngine/move/Move.h"
#include "ChessEngine/masks/PawnBitmasks.h"
#include "ChessEngine/perft/SearchMemory.h"
#include "ChessEngine/movegen/Movelist.h"

#include "ChessEngine/movegen/RayLogic.h"

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
        perft::SearchMemory& searchMemory
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

}