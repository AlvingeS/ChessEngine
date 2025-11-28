#pragma once

#include "logic/attack_tables/attack_tables.h"

namespace model {
    class Position;
}

namespace logic {

class CheckDetection {

public:
    CheckDetection(const model::Position& pos);
    bool in_check(bool is_w) const;

private:
    const model::Position& pos_;
    ray_attack_table& line_ray_attack_table_;
    ray_attack_table& diag_ray_attack_table_;
    attack_table& knight_attack_table_;
    attack_table& w_pawn_capture_attack_table_;
    attack_table& b_pawn_capture_attack_table_;
};

} // namespace logic