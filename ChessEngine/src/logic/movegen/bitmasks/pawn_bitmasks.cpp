#include "logic/movegen/bitmasks/pawn_bitmasks.h"

#include "logic/movegen/utils/chess_utils.h"

namespace logic {

bitmask PawnBitmasks::apply_capture_pawn_moves(int ind, bool is_w) 
{
    bitmask capture_moves = 0ULL;

    int shift_up_left = is_w ? 9 : -9;
    int shift_up_right = is_w ? 7 : -7;

    if (ind + shift_up_left <= 63 && ind + shift_up_left >= 0) capture_moves |= (1ULL << (ind + shift_up_left));
    if (ind + shift_up_right <= 63 && ind + shift_up_right >= 0) capture_moves |= (1ULL << (ind + shift_up_right));

    return capture_moves;
}

bitmask PawnBitmasks::remove_wrap_around_pawn_moves(bitmask capture_moves, int ind) 
{
    bitmask not_a_file = ~ChessUtils::get_file_mask(7);
    bitmask not_h_file = ~ChessUtils::get_file_mask(0);

    switch (ChessUtils::file_from_bit_index(ind)) {
        case 0:
            capture_moves &= not_a_file;
            break;
        case 7:
            capture_moves &= not_h_file;
            break;
    }

    return capture_moves;
}

bitmask PawnBitmasks::get_capture_pawn_moves_bitmask(int ind, bool is_w) 
{
    bitmask capture_moves = apply_capture_pawn_moves(ind, is_w);
    capture_moves = remove_wrap_around_pawn_moves(capture_moves, ind);
    return capture_moves;
}

bitmask PawnBitmasks::get_straight_pawn_moves_bitmask(int ind, bool is_w) 
{
    bitmask straight_moves = 0ULL;

    int shift_up = is_w ? 8 : -8;
    int shift_up_twice = is_w ? 16 : -16;

    if (ind + shift_up <= 63 && ind + shift_up >= 0) straight_moves |= (1ULL << (ind + shift_up));
    if (is_w && (ind >= 8 && ind <= 15)) {
        if (ind + shift_up_twice <= 63 && ind + shift_up_twice >= 0) straight_moves |= (1ULL << (ind + shift_up_twice));
    } else if (!is_w && (ind >= 48 && ind <= 55)) {
        if (ind + shift_up_twice <= 63 && ind + shift_up_twice >= 0) straight_moves |= (1ULL << (ind + shift_up_twice));
    }
    return straight_moves;
}

std::vector<bitmask> PawnBitmasks::get_all_straight_pawn_move_bitmasks(bool is_w) 
{
    std::vector<bitmask> straigh_pawn_moves_bitmask;

    for (int i = 0; i < 64; i++) {
        straigh_pawn_moves_bitmask.push_back(get_straight_pawn_moves_bitmask(i, is_w));
    }

    return straigh_pawn_moves_bitmask;
}

std::vector<bitmask> PawnBitmasks::get_all_capture_pawn_move_bitmasks(bool is_w) 
{
    std::vector<bitmask> capture_pawn_moves_bitmask;

    for (int i = 0; i < 64; i++) {
        capture_pawn_moves_bitmask.push_back(get_capture_pawn_moves_bitmask(i, is_w));
    }

    return capture_pawn_moves_bitmask;
}

} // namespace logic