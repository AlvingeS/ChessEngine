#pragma once

#include "model/types.h"

namespace model {
    class Movelist;
    class Position;
}

namespace logic {

class KnightGen {

public:
    KnightGen(const model::Position& pos);
    void generate(bool is_w, model::Movelist& movelist);

private:
    const model::Position& pos_;
    attack_table& knight_attack_table_;
};

} // namespace logic