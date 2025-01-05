#pragma once

#include "ChessEngine/src/logic/movegen/RayLogic.h"
#include "ChessEngine/src/logic/movegen/bitmasks/RayBitmasks.h"

#include "ChessEngine/src/model/move/Move.h"
#include "ChessEngine/src/model/move/Movelist.h"
#include "ChessEngine/src/model/board/Board.h"

namespace logic {

class BishopGenerator {

public:
    BishopGenerator(model::Board& board);
    
    void generate(
        bool isWhite, 
        model::Movelist& movelist
    );

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;
    std::vector<DiagonalRays> _diagonalRayBitmasks;
};

} // namespace logic