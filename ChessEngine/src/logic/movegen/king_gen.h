#pragma once

#include "model/types.h"

#include <vector>

namespace model {
    class Bitboards;
    class StateBitmasks;
    class Board;
    class Movelist;
}

namespace logic {

class KingGenerator {

public:
    KingGenerator(model::Board& board);
    void generate(bool isWhite, model::Movelist& moveList);

private:
    const model::Bitboards& _bitboards;
    const model::StateBitmasks& _stateBitmasks;
    std::vector<bitmask> _kingBitmasks;
};

} // namespace logic