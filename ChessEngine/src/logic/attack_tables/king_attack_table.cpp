#include "logic/attack_tables/king_attack_table.h"

#include "logic/utils.h"

namespace 
{

// Builds up all king moves without considering wrapping around the board
bitmask get_king_attack_mask(sq_idx sq) 
{
    bitmask attack_mask = 0ULL;

    if (sq + 8 <= 63) attack_mask |= (1ULL << (sq + 8));
    if (sq + 1 <= 63) attack_mask |= (1ULL << (sq + 1));
    if (sq - 8 >= 0)  attack_mask |= (1ULL << (sq - 8));
    if (sq - 1 >= 0)  attack_mask |= (1ULL << (sq - 1));
    if (sq + 7 <= 63) attack_mask |= (1ULL << (sq + 7));
    if (sq + 9 <= 63) attack_mask |= (1ULL << (sq + 9));
    if (sq - 7 >= 0)  attack_mask |= (1ULL << (sq - 7));
    if (sq - 9 >= 0)  attack_mask |= (1ULL << (sq - 9));

    return attack_mask;
}

// Applies reversed file masks to attack_mask to prevent looping around the board
void remove_wrap_around_from_attack_mask(bitmask& attack_mask, sq_idx sq) 
{
    bitmask all_files_except_a_mask = ~logic::utils::get_file_mask(7);
    bitmask all_files_except_h_mask = ~logic::utils::get_file_mask(0);

    switch (logic::utils::file_from_sq(sq)) {
        case 0: // If king is on file 0 (H), remove overlap that happened in A
            attack_mask &= all_files_except_a_mask;
            break;
        case 7: // If king is on file 7 (A), remove overlap that happened in H
            attack_mask &= all_files_except_h_mask;
            break;
    }
}

} // namespace

namespace logic::attack_tables {

void fill_king_attack_table(attack_table& king_attack_table) 
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        bitmask king_attack_mask = get_king_attack_mask(sq);
        remove_wrap_around_from_attack_mask(king_attack_mask, sq);
        king_attack_table[sq] = king_attack_mask;
    }
}

} // namespace logic::attack_tables