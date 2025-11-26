#pragma once

#include "model/types.h"
#include "model/position/piece_type.h"

#include <vector>

namespace model {

class Bitboards {

public:
    Bitboards();

    const bitboard& get_bb_from_piece_type(Piece::Type piece_type) const;
    const bitboard& get_bb_from_idx(int idx) const;
    
    void clear_piece_type_bit(sq_idx sq, Piece::Type piece_type);
    void set_piece_type_bit(sq_idx sq, Piece::Type piece_type);
    void reset_bitboards();

    // Set, clear and get for all individual piece types / bbs
    #define DEFINE_BITBOARD_MACROS(PIECE_NAME, VARIABLE_NAME) \
        inline void set_##PIECE_NAME##_bit(int square) \
        { \
            VARIABLE_NAME |= (1ULL << square); \
        } \
        inline void clear_##PIECE_NAME##_bit(int square) \
        { \
            VARIABLE_NAME &= ~(1ULL << square); \
        } \
        inline const bitboard& get_##PIECE_NAME##_bb() const \
        { \
            return VARIABLE_NAME; \
        }

    DEFINE_BITBOARD_MACROS(w_pawns, w_pawns_bb_)
    DEFINE_BITBOARD_MACROS(w_knights, w_knights_bb_)
    DEFINE_BITBOARD_MACROS(w_bishops, w_bishops_bb_)
    DEFINE_BITBOARD_MACROS(w_rooks, w_rooks_bb_)
    DEFINE_BITBOARD_MACROS(w_queens, w_queens_bb_)
    DEFINE_BITBOARD_MACROS(w_king, w_king_bb_)
    DEFINE_BITBOARD_MACROS(b_pawns, b_pawns_bb_)
    DEFINE_BITBOARD_MACROS(b_knights, b_knights_bb_)
    DEFINE_BITBOARD_MACROS(b_bishops, b_bishops_bb_)
    DEFINE_BITBOARD_MACROS(b_rooks, b_rooks_bb_)
    DEFINE_BITBOARD_MACROS(b_queens, b_queens_bb_)
    DEFINE_BITBOARD_MACROS(b_king, b_king_bb_)

private:
    std::vector<bitboard*> bbs_;

    bitboard w_pawns_bb_;
    bitboard w_knights_bb_;
    bitboard w_bishops_bb_;
    bitboard w_rooks_bb_;
    bitboard w_queens_bb_;
    bitboard w_king_bb_;

    bitboard b_pawns_bb_;
    bitboard b_knights_bb_;
    bitboard b_bishops_bb_;
    bitboard b_rooks_bb_;
    bitboard b_queens_bb_;
    bitboard b_king_bb_;
};

} // namespace model

#include "model/position/bitboards.inl"