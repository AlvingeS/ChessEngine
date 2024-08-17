#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/move/Move.h"
#include "ChessEngine/masks/RayBitmasks.h"
#include "ChessEngine/masks/KnightBitmasks.h"
#include "ChessEngine/masks/PawnBitmasks.h"
#include "ChessEngine/board/Bitboards.h"

#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {

class CheckDetection {

public:
    // Constructor
    CheckDetection(board::Bitboards& bitboards, RayLogic* rayLogic);
    
    // Public member functions
    bool isInCheck(bool isWhite);

private:
    // Private member functions
    board::Bitboards& _bitboards;
    RayLogic* _rayLogic;

    // Memory
    std::vector<masks::StraightRays> _straightRayBitmasks;
    std::vector<masks::DiagonalRays> _diagonalRayBitmasks;
    std::vector<bitmask> _knightBitmasks;
    std::vector<bitmask> _whitePawnCaptureMoveBitmasks;
    std::vector<bitmask> _blackPawnCaptureMoveBitmasks;

};

} // namespace movegen