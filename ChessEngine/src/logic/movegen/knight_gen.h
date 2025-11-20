#pragma once

#include "model/types.h"
#include <vector>

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
    void generate(bool is_w, model::Movelist& movelist);

private:
    const model::Bitboards& bitboards_;
    const model::StateBitmasks& state_bitmasks_;
    
    attack_table& knight_attack_table_;
};

} // namespace logic