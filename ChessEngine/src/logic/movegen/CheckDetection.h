#pragma once

#include "ChessEngine/src/model/move/Movelist.h"
#include "ChessEngine/src/logic/movegen/RayLogic.h"

#include "ChessEngine/src/model/move/Move.h"
#include "ChessEngine/src/logic/movegen/bitmasks/RayBitmasks.h"
#include "ChessEngine/src/logic/movegen/bitmasks/KnightBitmasks.h"
#include "ChessEngine/src/logic/movegen/bitmasks/PawnBitmasks.h"
#include "ChessEngine/src/model/board/Board.h"

namespace logic {

class CheckDetection {

public:
    CheckDetection(model::Board& board);
    bool isInCheck(bool isWhite) const;

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;

    std::vector<StraightRays> _straightRayBitmasks;
    std::vector<DiagonalRays> _diagonalRayBitmasks;
    std::vector<bitmask> _knightBitmasks;
    std::vector<bitmask> _whitePawnCaptureMoveBitmasks;
    std::vector<bitmask> _blackPawnCaptureMoveBitmasks;
};

} // namespace logic