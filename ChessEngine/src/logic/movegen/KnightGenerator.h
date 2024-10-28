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
        const representation::board::Bitboards& bitboards,
        const representation::board::GameStateBitmasks& gameStateBitmasks
    );
    
    // Public member functions
    void generate(bool isWhite, representation::move::Movelist& moveListRef);

private:
    // Private member functions
    const representation::board::Bitboards& _bitboardsRef;
    const representation::board::GameStateBitmasks& _gameStateBitmasksRef;

    // Memory
    std::vector<bitmask> _knightBitmasks;
};

} // namespace movegen
} // namespace logic