#pragma once

#include "ChessEngine/src/game/movegen/Movelist.h"
#include "ChessEngine/src/game/movegen/RayLogic.h"

#include "ChessEngine/src/game/board/Bitboards.h"
#include "ChessEngine/src/game/board/GameStateBitmasks.h"
#include "ChessEngine/src/game/move/Move.h"
#include "ChessEngine/src/game/masks/KnightBitmasks.h"

namespace movegen {

class KnightGenerator {

public:
    // Constructor
    KnightGenerator(
        const board::Bitboards& bitboards,
        const board::GameStateBitmasks& gameStateBitmasks
    );
    
    // Public member functions
    void generate(bool isWhite, Movelist& moveListRef);

private:
    // Private member functions
    const board::Bitboards& _bitboardsRef;
    const board::GameStateBitmasks& _gameStateBitmasksRef;

    // Memory
    std::vector<bitmask> _knightBitmasks;
};

}