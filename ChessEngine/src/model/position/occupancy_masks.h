#pragma once

#include "model/types.h"

namespace model {

class Bitboards;

class OccupancyMasks {

public:
    OccupancyMasks(const Bitboards& bitboards);

    const bitmask& get_w_pieces_mask() const;
    const bitmask& get_b_pieces_mask() const;
    const bitmask& get_occupied_sqrs_mask() const;
    const bitmask& get_free_sqrs_mask() const;
    
    void set_w_pieces_bit(sq_idx sq);
    void clear_w_pieces_bit(sq_idx sq);
    void set_b_pieces_bit(sq_idx sq);
    void clear_b_pieces_bit(sq_idx sq);
    void update_occupancy_masks();
    void fill_w_pieces_mask_from_bbs(const Bitboards& bbs);
    void fill_b_pieces_mask_from_bbs(const Bitboards& bbs);

private:
    void update_occupied_sqrs_mask();
    void update_free_sqrs_mask();

    bitmask w_pieces_mask_;
    bitmask b_pieces_mask_;
    bitmask occupied_sqrs_mask_;
    bitmask free_sqrs_mask_;
};

} // namespace model

#include "model/position/occupancy_masks.inl"