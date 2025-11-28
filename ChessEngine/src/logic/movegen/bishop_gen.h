#pragma once

#include "logic/attack_tables/attack_tables.h"

namespace model {
    class Movelist;
    class Position;
}

namespace logic {

class BishopGen {

public:
    BishopGen(const model::Position& pos);
    
    void generate(bool is_w, model::Movelist& movelist);

private:
    const model::Position& pos_;
    ray_attack_table& diag_ray_attack_table_;
};

} // namespace logic