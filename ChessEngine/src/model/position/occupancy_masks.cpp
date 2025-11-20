#include "model/position/occupancy_masks.h"

#include "model/position/bitboards.h"

namespace model {

OccupancyMasks::OccupancyMasks(const Bitboards& bbs) 
{
    fill_w_pieces_mask_from_bbs(bbs);
    fill_b_pieces_mask_from_bbs(bbs);
    update_occupancy_masks();
}

void OccupancyMasks::fill_w_pieces_mask_from_bbs(const Bitboards& bbs) 
{
    bitmask w_pieces_mask = 0ULL;
    for (int i = 0; i < 6; i++) {
        w_pieces_mask |= bbs.get_bitboard_from_index(i);
    }

    w_pieces_mask_ = w_pieces_mask;
}

void OccupancyMasks::fill_b_pieces_mask_from_bbs(const Bitboards& bbs) 
{
    bitmask b_pieces_mask = 0ULL;
    for (int i = 6; i < 12; i++) {
        b_pieces_mask |= bbs.get_bitboard_from_index(i);
    }

    b_pieces_mask_ = b_pieces_mask;
}

void OccupancyMasks::update_occupancy_masks() 
{
    update_occupied_sqrs_mask();
    update_free_sqrs_mask();
}

} // namespace model