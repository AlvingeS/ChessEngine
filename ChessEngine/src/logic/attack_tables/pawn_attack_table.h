#pragma once

#include "model/types.h"

namespace logic::attack_tables {

void fill_w_pawn_quiet_attack_table(attack_table& w_pawn_quiet_attack_table);
void fill_w_pawn_capture_attack_table(attack_table& w_pawn_capture_attack_table);
void fill_b_pawn_quiet_attack_table(attack_table& b_pawn_quiet_attack_table);
void fill_b_pawn_capture_attack_table(attack_table& b_pawn_capture_attack_table);

} // namespace logic::attack_tables