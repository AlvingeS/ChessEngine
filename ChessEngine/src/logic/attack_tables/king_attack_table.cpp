#include "logic/attack_tables/king_attack_table.h"

#include "logic/movegen/utils/chess_utils.h"

namespace 
{

// Builds up all king moves without considering wrapping around the board
bitmask get_king_attack_mask(int sq_idx) 
{
    bitmask attack_mask = 0ULL;

    if (sq_idx + 8 <= 63) attack_mask |= (1ULL << (sq_idx + 8));
    if (sq_idx + 1 <= 63) attack_mask |= (1ULL << (sq_idx + 1));
    if (sq_idx - 8 >= 0)  attack_mask |= (1ULL << (sq_idx - 8));
    if (sq_idx - 1 >= 0)  attack_mask |= (1ULL << (sq_idx - 1));
    if (sq_idx + 7 <= 63) attack_mask |= (1ULL << (sq_idx + 7));
    if (sq_idx + 9 <= 63) attack_mask |= (1ULL << (sq_idx + 9));
    if (sq_idx - 7 >= 0)  attack_mask |= (1ULL << (sq_idx - 7));
    if (sq_idx - 9 >= 0)  attack_mask |= (1ULL << (sq_idx - 9));

    return attack_mask;
}

// Applies reversed file masks to attack_mask to prevent looping around the board
void remove_wrap_around_from_attack_mask(bitmask& attack_mask, int sq_idx) 
{
    bitmask all_files_except_a_mask = ~logic::ChessUtils::get_file_mask(7);
    bitmask all_files_except_h_mask = ~logic::ChessUtils::get_file_mask(0);

    switch (logic::ChessUtils::file_from_bit_idx(sq_idx)) {
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
    for (int sq_idx = 0; sq_idx < 64; sq_idx++) {
        bitmask king_attack_mask = get_king_attack_mask(sq_idx);
        remove_wrap_around_from_attack_mask(king_attack_mask, sq_idx);
        king_attack_table[sq_idx] = king_attack_mask;
    }
}

} // namespace logic::attack_tables