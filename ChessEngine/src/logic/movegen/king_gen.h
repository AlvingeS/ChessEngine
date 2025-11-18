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
    void generate(bool is_w, model::Movelist& movelist);

private:
    const model::Bitboards& bitboards_;
    const model::StateBitmasks& state_bitmasks_;
    std::vector<bitmask> king_bitmasks_;
};

} // namespace logic