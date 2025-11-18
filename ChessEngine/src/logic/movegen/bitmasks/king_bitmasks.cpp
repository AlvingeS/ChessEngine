#include "logic/movegen/bitmasks/king_bitmasks.h"

#include "logic/movegen/utils/chess_utils.h"

namespace logic {

// Applies all king moves without considering looping around the board
bitmask KingBitmasks::apply_king_moves(int ind) 
{
    bitmask moves = 0ULL;

    if (ind + 8 <= 63) moves |= (1ULL << (ind + 8));
    if (ind + 1 <= 63) moves |= (1ULL << (ind + 1));
    if (ind - 8 >= 0) moves |= (1ULL << (ind - 8));
    if (ind - 1 >= 0) moves |= (1ULL << (ind - 1));
    if (ind + 7 <= 63) moves |= (1ULL << (ind + 7));
    if (ind + 9 <= 63) moves |= (1ULL << (ind + 9));
    if (ind - 7 >= 0) moves |= (1ULL << (ind - 7));
    if (ind - 9 >= 0) moves |= (1ULL << (ind - 9));

    return moves;
}

// Applies file masks to king moves to prevent looping around the board
bitmask KingBitmasks::remove_wrap_around_king_moves(bitmask moves, int ind) 
{
    bitmask not_a_file = ~ChessUtils::get_file_mask(7);
    bitmask not_h_file = ~ChessUtils::get_file_mask(0);

    switch (ChessUtils::file_from_bit_index(ind)) {
        case 0:
            moves &= not_a_file;
            break;
        case 7:
            moves &= not_h_file;
            break;
    }

    return moves;
}

bitmask KingBitmasks::get_king_bitmask(int ind) 
{
    bitmask moves = apply_king_moves(ind);
    moves = remove_wrap_around_king_moves(moves, ind);
    return moves;
}

const std::vector<bitmask> KingBitmasks::get_all_king_bitmasks() 
{
    std::vector<bitmask> king_bitmasks;

    for (int i = 0; i < 64; i++) {
        king_bitmasks.push_back(get_king_bitmask(i));
    }

    return king_bitmasks;
}

} // namespace logic