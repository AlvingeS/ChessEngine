#include "logic/attack_tables/attack_tables.h"

#include "logic/attack_tables/king_attack_table.h"
#include "logic/attack_tables/knight_attack_table.h"
#include "logic/attack_tables/pawn_attack_table.h"
#include "logic/attack_tables/rays_attack_table.h"

namespace logic::attack_tables 
{

attack_table king{};
attack_table knight{};
attack_table w_pawn_quiet{};
attack_table w_pawn_capture{};
attack_table b_pawn_quiet{};
attack_table b_pawn_capture{};
ray_attack_table rays{};

void init_attack_tables() 
{
    fill_king_attack_table(king);
    fill_knight_attack_table(knight);
    fill_w_pawn_quiet_attack_table(w_pawn_quiet);
    fill_w_pawn_capture_attack_table(w_pawn_capture);
    fill_b_pawn_quiet_attack_table(b_pawn_quiet);
    fill_b_pawn_capture_attack_table(b_pawn_capture);
    fill_ray_attack_table(rays);
}

} // namespace logic::attack_tables