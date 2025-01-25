#pragma once

#include "ChessEngine/src/common.h"

// FIXME: We cant have a dependency to perft
#include "ChessEngine/src/engine/search/SearchMemory.h"

namespace model {
    class Board;
    class Movelist;
    class Bitboards;
    class StateBitmasks;
}

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