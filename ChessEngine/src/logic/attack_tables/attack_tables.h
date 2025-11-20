#pragma once

#include "model/types.h"

namespace logic::attack_tables
{

extern attack_table king;
extern attack_table knight;
extern attack_table w_pawn_quiet;
extern attack_table w_pawn_capture;
extern attack_table b_pawn_quiet;
extern attack_table b_pawn_capture;
extern ray_attack_table line_ray;
extern ray_attack_table diag_ray;

void init_attack_tables();

} // namespace logic::attack_tables