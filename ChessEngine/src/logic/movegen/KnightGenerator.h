#pragma once

#include "ChessEngine/src/common.h"

namespace model {
    class Movelist;
    class Board;
    class Bitboards;
    class StateBitmasks;
}

namespace logic {

class KnightGenerator {

public:
    KnightGenerator(model::Board& board);
    void generate(bool isWhite, model::Movelist& moveList);

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;
    std::vector<bitmask> _knightBitmasks;
};

} // namespace logic