#pragma once

#include "model/types.h"

namespace model {

class Bitboards;

class StateBitmasks {

public:
    StateBitmasks(const Bitboards& bitboards);

    bool w_pieces_bit_is_set(int index) const;
    bool b_pieces_bit_is_set(int index) const;
    const bitmask& get_w_pieces_bitmask() const;
    const bitmask& get_b_pieces_bitmask() const;
    const bitmask& get_occupied_pieces_bitmask() const;
    const bitmask& get_empty_squares_bitmask() const;
    
    void set_w_pieces_bit(int index);
    void clear_w_pieces_bit(int index);
    void set_b_pieces_bit(int index);
    void clear_b_pieces_bit(int index);
    void update_occupied_and_empty_squares_bitmasks();
    void fill_w_pieces_bitmask_from_bitboards(const Bitboards& bitboards);
    void fill_b_pieces_bitmask_from_bitboards(const Bitboards& bitboards);

private:
    void update_occupied_pieces_bitmask();
    void update_empty_squares_bitmask();

    bitmask w_pieces_bitmask_;
    bitmask b_pieces_bitmask_;
    bitmask occupied_pieces_bitmask_;
    bitmask empty_squares_bitmask_;
};

} // namespace model

#include "model/position/state_bitmasks.inl"