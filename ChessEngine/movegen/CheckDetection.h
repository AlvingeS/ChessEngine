#pragma once

#include "ChessEngine/common.h"

#include "ChessEngine/move/Move.h"
#include "ChessEngine/masks/RayBitMasks.h"
#include "ChessEngine/masks/KnightBitMasks.h"
#include "ChessEngine/masks/PawnBitMasks.h"
#include "ChessEngine/board/BitBoards.h"

#include "RayLogic.h"
#include "CommonLogic.h"

namespace movegen {

class CheckDetection {

public:
    // Constructor
    CheckDetection(board::BitBoards& bitboards, RayLogic* rayLogic);
    
    // Public member functions
    bool isInCheck(bool isWhite);

private:
    // Private member functions
    board::BitBoards& _bitboards;
    RayLogic* _rayLogic;

    // Memory
    std::vector<masks::StraightRays> _straightRayBitmasks;
    std::vector<masks::DiagonalRays> _diagonalRayBitmasks;
    std::vector<U64> _knightBitmasks;
    std::vector<U64> _whitePawnCaptureMoveBitmasks;
    std::vector<U64> _blackPawnCaptureMoveBitmasks;

};

} // namespace movegen