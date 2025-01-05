#pragma once

#include "ChessEngine/src/model/move/Movelist.h"
#include "ChessEngine/src/logic/movegen/RayLogic.h"

#include "ChessEngine/src/model/board/Board.h"

#include "ChessEngine/src/model/move/Move.h"
#include "ChessEngine/src/logic/movegen/bitmasks/PawnBitmasks.h"

// FIXME: We cant have a dependency to perft
#include "ChessEngine/src/engine/search/SearchMemory.h"


namespace logic {

class PawnGenerator {

public:
    PawnGenerator(model::Board& board);
    
    void generate(
        bool isWhite,
        model::Movelist& moveList,
        int currentDepth,
        engine::SearchMemory& searchMemory
    );

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;

    std::vector<bitmask> _whitePawnStraightMoveBitmasks;
    std::vector<bitmask> _blackPawnStraightMoveBitmasks;
    std::vector<bitmask> _whitePawnCaptureMoveBitmasks;
    std::vector<bitmask> _blackPawnCaptureMoveBitmasks;
};

} // namespace logic