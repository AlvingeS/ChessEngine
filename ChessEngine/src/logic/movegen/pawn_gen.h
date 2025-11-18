#pragma once

#include "model/types.h"
#include <vector>

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
        bitmask enpessantTarget
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