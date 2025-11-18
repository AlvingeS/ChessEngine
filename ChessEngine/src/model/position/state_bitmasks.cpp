#include "model/position/state_bitmasks.h"

#include "model/position/bitboards.h"

namespace model {

StateBitmasks::StateBitmasks(const Bitboards& bitboards) 
{
    fill_w_pieces_bitmask_from_bitboards(bitboards);
    fill_b_pieces_bitmask_from_bitboards(bitboards);
    update_occupied_and_empty_squares_bitmasks();
}

void StateBitmasks::fill_w_pieces_bitmask_from_bitboards(const Bitboards& bitboards) 
{
    bitmask w_all = 0;
    for (int i = 0; i < 6; i++) {
        w_all |= bitboards.get_bitboard_from_index(i);
    }

    w_pieces_bitmask_ = w_all;
}

void StateBitmasks::fill_b_pieces_bitmask_from_bitboards(const Bitboards& bitboards) 
{
    bitmask b_all = 0;
    for (int i = 6; i < 12; i++) {
        b_all |= bitboards.get_bitboard_from_index(i);
    }

    b_pieces_bitmask_ = b_all;
}

void StateBitmasks::update_occupied_and_empty_squares_bitmasks() 
{
    update_occupied_pieces_bitmask();
    update_empty_squares_bitmask();
}

} // namespace model