#pragma once

#include "model/types.h"

namespace model {
    class Position;
    class Movelist;
}

namespace logic {

class KingGen {

public:
    KingGen(const model::Position& pos);

    void generate(bool is_w, model::Movelist& movelist);

private:
    const model::Position& pos_;    
    attack_table& king_attack_table_;
};

} // namespace logic