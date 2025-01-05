#pragma once

#include "ChessEngine/src/model/board/Bitboards.h"
#include "ChessEngine/src/model/board/PieceMap.h"
#include "ChessEngine/src/model/board/StateBitmasks.h"
#include "ChessEngine/src/model/board/ZHasher.h"

namespace model {

struct Board {
    Bitboards bitboards;
    PieceMap pieceMap;
    StateBitmasks stateBitmasks;
    ZHasher zHasher;

    Board() 
        : pieceMap(bitboards)
        , stateBitmasks(bitboards)
        , zHasher(pieceMap) 
    {}
};

} // namespace model