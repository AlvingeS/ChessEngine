#pragma once

#include "model/types.h"

namespace model {
    class Movelist;
    class Position;
}

namespace logic {

class PawnGen {

public:
    PawnGen(const model::Position& position);
    
    void generate(bool is_w, model::Movelist& movelist);

private:
    const model::Position& pos_;
    attack_table& w_pawn_quiet_attack_table_;
    attack_table& w_pawn_capture_attack_table_;
    attack_table& b_pawn_quiet_attack_table_;
    attack_table& b_pawn_capture_attack_table_;
};

} // namespace logic