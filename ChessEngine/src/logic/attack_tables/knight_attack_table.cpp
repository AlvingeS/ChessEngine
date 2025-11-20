#include "logic/attack_tables/knight_attack_table.h"

#include "logic/movegen/utils/chess_utils.h"

namespace 
{

// Builds up all knight moves without considering wrapping around the board
bitmask get_knight_attack_mask(int sq_idx) 
{
    bitmask attack_mask = 0ULL;

    if (sq_idx + 15 <= 63)  attack_mask |= (1ULL << (sq_idx + 15));
    if (sq_idx + 6 <= 63)   attack_mask |= (1ULL << (sq_idx + 6));
    if (sq_idx - 10 >= 0)   attack_mask |= (1ULL << (sq_idx - 10));
    if (sq_idx - 17 >= 0)   attack_mask |= (1ULL << (sq_idx - 17));
    if (sq_idx - 15 >= 0)   attack_mask |= (1ULL << (sq_idx - 15));
    if (sq_idx - 6 >= 0)    attack_mask |= (1ULL << (sq_idx - 6));
    if (sq_idx + 10 <= 63)  attack_mask |= (1ULL << (sq_idx + 10));
    if (sq_idx + 17 <= 63)  attack_mask |= (1ULL << (sq_idx + 17));

    return attack_mask;
}

// Applies reversed file masks to attack_mask to prevent looping around the board
void remove_wrap_around_from_attack_mask(bitmask& attack_mask, int sq_idx) 
{
    bitmask a_file_mask = logic::ChessUtils::get_file_mask(7);
    bitmask b_file_mask = logic::ChessUtils::get_file_mask(6);
    bitmask all_files_except_ab_mask = ~(a_file_mask | b_file_mask);

    bitmask g_file_mask = logic::ChessUtils::get_file_mask(1);
    bitmask h_file_mask = logic::ChessUtils::get_file_mask(0);
    bitmask all_files_except_gh_mask = ~(g_file_mask | h_file_mask);

    switch (logic::ChessUtils::file_from_bit_index(sq_idx)) {
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
    for (int sq_idx = 0; sq_idx < 64; sq_idx++) {
        bitmask knight_attack_mask = get_knight_attack_mask(sq_idx);
        remove_wrap_around_from_attack_mask(knight_attack_mask, sq_idx);
        knight_attack_table[sq_idx] = knight_attack_mask;
    }
}

} // namespace logic::attack_tables