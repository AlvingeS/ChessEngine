#include "logic/attack_tables/attack_tables.h"

#include "logic/attack_tables/king_attack_table.h"
#include "logic/attack_tables/knight_attack_table.h"
#include "logic/attack_tables/pawn_attack_table.h"
#include "logic/attack_tables/line_ray_attack_table.h"
#include "logic/attack_tables/diag_ray_attack_table.h"

namespace logic::attack_tables 
{

attack_table king{};
attack_table knight{};
attack_table w_pawn_quiet{};
attack_table w_pawn_capture{};
attack_table b_pawn_quiet{};
attack_table b_pawn_capture{};
ray_attack_table line_ray{};
ray_attack_table diag_ray{};

void init_attack_tables() 
{
    fill_king_attack_table(king);
    fill_knight_attack_table(knight);
    fill_w_pawn_quiet_attack_table(w_pawn_quiet);
    fill_w_pawn_capture_attack_table(w_pawn_capture);
    fill_b_pawn_quiet_attack_table(b_pawn_quiet);
    fill_b_pawn_capture_attack_table(b_pawn_capture);
    fill_line_ray_attack_table(line_ray);
    fill_diag_ray_attack_table(diag_ray);
}

} // namespace logic::attack_tables