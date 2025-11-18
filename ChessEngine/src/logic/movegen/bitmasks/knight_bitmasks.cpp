#include "logic/movegen/bitmasks/knight_bitmasks.h"

#include "logic/movegen/utils/chess_utils.h"

namespace logic {

// Applies all knight moves without considering looping around the board
bitmask KnightBitmasks::apply_knight_moves(int ind) 
{
    bitmask moves = 0ULL;

    if (ind + 15 <= 63) moves |= (1ULL << (ind + 15));
    if (ind + 6 <= 63) moves |= (1ULL << (ind + 6));
    if (ind - 10 >= 0) moves |= (1ULL << (ind - 10));
    if (ind - 17 >= 0) moves |= (1ULL << (ind - 17));
    if (ind - 15 >= 0) moves |= (1ULL << (ind - 15));
    if (ind - 6 >= 0) moves |= (1ULL << (ind - 6));
    if (ind + 10 <= 63) moves |= (1ULL << (ind + 10));
    if (ind + 17 <= 63) moves |= (1ULL << (ind + 17));

    return moves;
}

// Applies file masks to knight moves to prevent looping around the board
bitmask KnightBitmasks::remove_wrap_around_knight_moves(bitmask moves, int ind) 
{
    bitmask a_file = ChessUtils::get_file_mask(7);
    bitmask b_file = ChessUtils::get_file_mask(6);
    bitmask not_a_or_b_file = ~(a_file | b_file);

    bitmask g_file = ChessUtils::get_file_mask(1);
    bitmask h_file = ChessUtils::get_file_mask(0);
    bitmask not_g_or_h_file = ~(g_file | h_file);

    switch (ChessUtils::file_from_bit_index(ind)) {
        case 0:
            moves &= not_a_or_b_file;
            break;
        case 1:
            moves &= not_a_or_b_file;
            break;
        case 6:
            moves &= not_g_or_h_file;
            break;
        case 7:
            moves &= not_g_or_h_file;
            break;
    }

    return moves;
}

bitmask KnightBitmasks::get_knight_bitmask(int ind) 
{
    bitmask moves = apply_knight_moves(ind);
    moves = remove_wrap_around_knight_moves(moves, ind);
    return moves;
}

const std::vector<bitmask> KnightBitmasks::get_all_knight_bitmasks() 
{
    std::vector<bitmask> knight_bitmasks;

    for (int i = 0; i < 64; i++) {
        knight_bitmasks.push_back(get_knight_bitmask(i));
    }

    return knight_bitmasks;
}

} // namespace logic