#include "logic/attack_tables/pawn_attack_table.h"

#include "logic/movegen/utils/chess_utils.h"

namespace 
{

// Builds up all white quiete pawn moves
bitmask get_w_pawn_quiet_attack_mask(sq_idx sq) 
{
    bitmask w_quiet_attack_mask = 0ULL;

    int step     = 8;
    int dbl_step = 16;

    if (sq + step <= 63 && sq + step >= 0) w_quiet_attack_mask |= (1ULL << (sq + step));

    if (sq >= 8 && sq <= 15) {
        if (sq + dbl_step <= 63 && sq + dbl_step >= 0) w_quiet_attack_mask |= (1ULL << (sq + dbl_step));
    }

    return w_quiet_attack_mask;
}

// Builds up all black quiete pawn moves
bitmask get_b_pawn_quiet_attack_mask(sq_idx sq) 
{
    bitmask b_quiet_attack_mask = 0ULL;

    int step     = -8;
    int dbl_step = -16;

    if (sq + step <= 63 && sq + step >= 0) b_quiet_attack_mask |= (1ULL << (sq + step));

    if (sq >= 48 && sq <= 55) {
        if (sq + dbl_step <= 63 && sq + dbl_step >= 0) b_quiet_attack_mask |= (1ULL << (sq + dbl_step));
    }

    return b_quiet_attack_mask;
}

// Builds up all pawn capture moves without considering wrapping around the board
bitmask get_w_pawn_capture_attack_mask(sq_idx sq) 
{
    bitmask capture_attack_mask = 0ULL;

    int step_up_left  = 9;
    int step_up_right = 7;

    if (sq + step_up_left  <= 63 && sq + step_up_left  >= 0) capture_attack_mask |= (1ULL << (sq + step_up_left));
    if (sq + step_up_right <= 63 && sq + step_up_right >= 0) capture_attack_mask |= (1ULL << (sq + step_up_right));

    return capture_attack_mask;
}

// Builds up all pawn capture moves without considering wrapping around the board
bitmask get_b_pawn_capture_attack_mask(sq_idx sq) 
{
    bitmask capture_attack_mask = 0ULL;
    
    int step_up_left  = -9;
    int step_up_right = -7;

    if (sq + step_up_left  <= 63 && sq + step_up_left  >= 0) capture_attack_mask |= (1ULL << (sq + step_up_left));
    if (sq + step_up_right <= 63 && sq + step_up_right >= 0) capture_attack_mask |= (1ULL << (sq + step_up_right));

    return capture_attack_mask;
}

// Applies reversed file masks to attack_mask to prevent looping around the board
void remove_wrap_around_from_capture_attack_mask(bitmask& capture_attack_mask, sq_idx sq) 
{
    bitmask all_files_except_a_mask = ~logic::chess_utils::get_file_mask(7);
    bitmask all_files_except_h_mask = ~logic::chess_utils::get_file_mask(0);

    switch (logic::chess_utils::file_from_sq(sq)) {
        case 0: // If pawn is on file 0 (H), remove overlap that happened in A
            capture_attack_mask &= all_files_except_a_mask;
            break;
        case 7: // If pawn is on file 7 (A), remove overlap that happened in H
            capture_attack_mask &= all_files_except_h_mask;
            break;
    }
}

} // namespace

namespace logic::attack_tables {

void fill_w_pawn_capture_attack_table(attack_table& w_pawn_capture_attack_table) 
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        bitmask pawn_capture_attack_mask = get_w_pawn_capture_attack_mask(sq);
        remove_wrap_around_from_capture_attack_mask(pawn_capture_attack_mask, sq);
        w_pawn_capture_attack_table[sq] = pawn_capture_attack_mask;
    }
}

void fill_b_pawn_capture_attack_table(attack_table& b_pawn_capture_attack_table) 
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        bitmask pawn_capture_attack_mask = get_b_pawn_capture_attack_mask(sq);
        remove_wrap_around_from_capture_attack_mask(pawn_capture_attack_mask, sq);
        b_pawn_capture_attack_table[sq] = pawn_capture_attack_mask;
    }
}

void fill_w_pawn_quiet_attack_table(attack_table& w_pawn_quiet_attack_table) 
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        w_pawn_quiet_attack_table[sq] = get_w_pawn_quiet_attack_mask(sq);
    }
}

void fill_b_pawn_quiet_attack_table(attack_table& b_pawn_quiet_attack_table) 
{
    for (sq_idx sq = 0; sq < 64; sq++) {
        b_pawn_quiet_attack_table[sq] = get_b_pawn_quiet_attack_mask(sq);
    }
}

} // namespace logic::attack_tables