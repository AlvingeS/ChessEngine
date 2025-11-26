#include "logic/attack_tables/knight_attack_table.h"

#include "logic/movegen/utils/chess_utils.h"

namespace 
{

// Builds up all knight moves without considering wrapping around the board
bitmask get_knight_attack_mask(sq_idx sq) 
{
    bitmask attack_mask = 0ULL;

    if (sq + 15 <= 63)  attack_mask |= (1ULL << (sq + 15));
    if (sq + 6 <= 63)   attack_mask |= (1ULL << (sq + 6));
    if (sq - 10 >= 0)   attack_mask |= (1ULL << (sq - 10));
    if (sq - 17 >= 0)   attack_mask |= (1ULL << (sq - 17));
    if (sq - 15 >= 0)   attack_mask |= (1ULL << (sq - 15));
    if (sq - 6 >= 0)    attack_mask |= (1ULL << (sq - 6));
    if (sq + 10 <= 63)  attack_mask |= (1ULL << (sq + 10));
    if (sq + 17 <= 63)  attack_mask |= (1ULL << (sq + 17));

    return attack_mask;
}

// Applies reversed file masks to attack_mask to prevent looping around the board
void remove_wrap_around_from_attack_mask(bitmask& attack_mask, sq_idx sq) 
{
    bitmask a_file_mask = logic::chess_utils::get_file_mask(7);
    bitmask b_file_mask = logic::chess_utils::get_file_mask(6);
    bitmask all_files_except_ab_mask = ~(a_file_mask | b_file_mask);

    bitmask g_file_mask = logic::chess_utils::get_file_mask(1);
    bitmask h_file_mask = logic::chess_utils::get_file_mask(0);
    bitmask all_files_except_gh_mask = ~(g_file_mask | h_file_mask);

    switch (logic::chess_utils::file_from_sq(sq)) {
        case 0: // If knight is on file 0 (H), remove overlap that happened in A or B
            attack_mask &= all_files_except_ab_mask;
            break;
        case 1: // If knight is on file 1 (G), remove overlap that happened in A or B
            attack_mask &= all_files_except_ab_mask;
            break;
        case 6: // If knight is on file 7 (B), remove overlap that happened in G or H
            attack_mask &= all_files_except_gh_mask;
            break;
        case 7: // If knight is on file 7 (A), remove overlap that happened in G or H
            attack_mask &= all_files_except_gh_mask;
            break;
    }
}

} // namespace

namespace logic::attack_tables {

void fill_knight_attack_table(attack_table& knight_attack_table) 
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        bitmask knight_attack_mask = get_knight_attack_mask(sq);
        remove_wrap_around_from_attack_mask(knight_attack_mask, sq);
        knight_attack_table[sq] = knight_attack_mask;
    }
}

} // namespace logic::attack_tables