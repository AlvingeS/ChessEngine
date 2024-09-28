#pragma once

#include "ChessEngine/src/game/movegen/Movelist.h"
#include "ChessEngine/src/game/movegen/RayLogic.h"

#include "ChessEngine/src/game/move/Move.h"
#include "ChessEngine/src/game/masks/RayBitmasks.h"
#include "ChessEngine/src/game/masks/KnightBitmasks.h"
#include "ChessEngine/src/game/masks/PawnBitmasks.h"
#include "ChessEngine/src/game/board/Bitboards.h"
#include "ChessEngine/src/game/board/GameStateBitmasks.h"

namespace movegen {

class CheckDetection {

public:
    // Constructor
    CheckDetection(const board::Bitboards& bitboards, const board::GameStateBitmasks& gameStateBitmasks);
    
    // Public member functions
    bool isInCheck(bool isWhite) const;

private:
    // Private member functions
    const board::Bitboards& _bitboardsRef;
    const board::GameStateBitmasks& _gameStateBitmasksRef;

    // Memory
    std::vector<masks::StraightRays> _straightRayBitmasks;
    std::vector<masks::DiagonalRays> _diagonalRayBitmasks;
    std::vector<bitmask> _knightBitmasks;
    std::vector<bitmask> _whitePawnCaptureMoveBitmasks;
    std::vector<bitmask> _blackPawnCaptureMoveBitmasks;

};

} // namespace movegen