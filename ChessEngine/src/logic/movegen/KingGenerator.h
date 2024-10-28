#pragma once

#include "ChessEngine/src/model/move/Movelist.h"
#include "ChessEngine/src/logic/movegen/RayLogic.h"

#include "ChessEngine/src/model/board/Bitboards.h"
#include "ChessEngine/src/model/board/GameStateBitmasks.h"

#include "ChessEngine/src/model/move/Move.h"
#include "ChessEngine/src/logic/movegen/bitmasks/KingBitmasks.h"

namespace logic {

class KingGenerator {

public:
    // Constructor
    KingGenerator(
        const model::Bitboards& bitboards,
        const model::GameStateBitmasks& gameStateBitmasks
    );
    
    // Public member functions
    void generate(bool isWhite, model::Movelist& moveListRef);

private:
    // Private member functions
    const model::Bitboards& _bitboardsRef;
    const model::GameStateBitmasks& _gameStateBitmasksRef;

    // Memory
    std::vector<bitmask> _kingBitmasks;
};

} // namespace logic