#pragma once

#include "model/types.h"
#include "model/position/piece_type.h"

#include <vector>

namespace model {

class Bitboards {

public:
    Bitboards();

    const bitboard& get_bitboard_from_piece_type(Piece::Type piece_type) const;
    const bitboard& get_bitboard_from_index(int index) const;
    bool piece_type_bit_is_set(int index, Piece::Type piece_type) const;
    
    void clear_piece_type_bit(int index, Piece::Type piece_type);
    void set_piece_type_bit(int index, Piece::Type piece_type);
    void reset_bitboards();

    // Set, clear and get for all individual piece types / bitboards
    #define DEFINE_BITBOARD_MACROS(PIECE_NAME, VARIABLE_NAME) \
        inline void set_##PIECE_NAME##_bit(int square) \
        { \
            VARIABLE_NAME |= (1ULL << square); \
        } \
        inline void clear_##PIECE_NAME##_bit(int square) \
        { \
            VARIABLE_NAME &= ~(1ULL << square); \
        } \
        inline const bitboard& get_##PIECE_NAME##_bitboard() const \
        { \
            return VARIABLE_NAME; \
        }

    DEFINE_BITBOARD_MACROS(w_pawns, w_pawns_bitboard_)
    DEFINE_BITBOARD_MACROS(w_knights, w_knights_bitboard_)
    DEFINE_BITBOARD_MACROS(w_bishops, w_bishops_bitboard_)
    DEFINE_BITBOARD_MACROS(w_rooks, w_rooks_bitboard_)
    DEFINE_BITBOARD_MACROS(w_queens, w_queens_bitboard_)
    DEFINE_BITBOARD_MACROS(w_king, w_king_bitboard_)
    DEFINE_BITBOARD_MACROS(b_pawns, b_pawns_bitboard_)
    DEFINE_BITBOARD_MACROS(b_knights, b_knights_bitboard_)
    DEFINE_BITBOARD_MACROS(b_bishops, b_bishops_bitboard)
    DEFINE_BITBOARD_MACROS(b_rooks, b_rooks_bitboard)
    DEFINE_BITBOARD_MACROS(b_queens, b_queens_bitboard)
    DEFINE_BITBOARD_MACROS(b_king, b_king_bitboard)

private:
    std::vector<bitboard*> bitboards_;

    bitboard w_pawns_bitboard_;
    bitboard w_knights_bitboard_;
    bitboard w_bishops_bitboard_;
    bitboard w_rooks_bitboard_;
    bitboard w_queens_bitboard_;
    bitboard w_king_bitboard_;

    bitboard b_pawns_bitboard_;
    bitboard b_knights_bitboard_;
    bitboard b_bishops_bitboard;
    bitboard b_rooks_bitboard;
    bitboard b_queens_bitboard;
    bitboard b_king_bitboard;
};

} // namespace model

#include "model/position/bitboards.inl"