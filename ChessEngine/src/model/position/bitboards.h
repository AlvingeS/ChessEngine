#pragma once

#include "model/types.h"

#include <cassert>

namespace model {

class Bitboards {

public:
    Bitboards();

    void set_bit_at(sq_t sq, PieceType piece_type);
    void clr_bit_at(sq_t sq, PieceType piece_type);
    bool get_bit_at(sq_t sq, PieceType PieceType) const;
    const bitboard& get(PieceType piece_type) const;
    
    bool get_w_bit(sq_t sq) const;
    bitboard get_w() const;

    bool get_b_bit(sq_t sq) const;
    bitboard get_b() const;

    bool get_occ_bit(sq_t sq) const;
    bitboard get_occ() const;
    
    bool get_empty_bit(sq_t sq) const;
    bitboard get_empty() const;

    PieceType get_piece_type_at(sq_t sq, bool is_w) const;
    PieceType get_piece_type_at(sq_t sq) const;
    void reset_bitboards();

private:
    bitboard bbs_[12];
};

} // namespace model

#include "model/position/bitboards.inl"