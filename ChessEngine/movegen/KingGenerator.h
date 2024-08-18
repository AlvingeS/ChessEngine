#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/board/Bitboards.h"
#include "ChessEngine/board/GameStateBitmasks.h"
#include "ChessEngine/move/Move.h"
#include "ChessEngine/masks/KingBitmasks.h"
#include "ChessEngine/movegen/Movelist.h"
#include "RayLogic.h"

namespace movegen {

class KingGenerator {

public:
    // Constructor
    KingGenerator(
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
    std::vector<bitmask> _kingBitmasks;
};

} // namespace movegen