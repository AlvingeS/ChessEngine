#pragma once

#include "ChessEngine/src/representation/move/Movelist.h"
#include "ChessEngine/src/logic/movegen/RayLogic.h"

#include "ChessEngine/src/representation/board/Bitboards.h"
#include "ChessEngine/src/representation/board/GameStateBitmasks.h"

#include "ChessEngine/src/representation/move/Move.h"
#include "ChessEngine/src/logic/movegen/bitmasks/KnightBitmasks.h"

namespace logic {
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

} // namespace movegen
} // namespace logic