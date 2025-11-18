#include "model/position/board.h"

namespace model {

Bitboards::Bitboards() 
{
    _bitboards.reserve(12);

    w_pawns_bitboard_ = 0x000000000000FF00ULL;
    w_knights_bitboard_ = 0x0000000000000042ULL;
    w_bishops_bitboard_ = 0x0000000000000024ULL;
    w_rooks_bitboard_ = 0x0000000000000081ULL;
    w_queens_bitboard_ = 0x0000000000000010ULL;
    w_king_bitboard_ = 0x0000000000000008ULL;

    b_pawns_bitboard_ = 0x00FF000000000000ULL;
    b_knights_bitboard_ = 0x4200000000000000ULL;
    b_bishops_bitboard = 0x2400000000000000ULL;
    b_rooks_bitboard = 0x8100000000000000ULL;
    b_queens_bitboard = 0x1000000000000000ULL;
    b_king_bitboard = 0x0800000000000000ULL;

    _bitboards.push_back(&w_pawns_bitboard_);
    _bitboards.push_back(&w_knights_bitboard_);
    _bitboards.push_back(&w_bishops_bitboard_);
    _bitboards.push_back(&w_rooks_bitboard_);
    _bitboards.push_back(&w_queens_bitboard_);
    _bitboards.push_back(&w_king_bitboard_);

    _bitboards.push_back(&b_pawns_bitboard_);
    _bitboards.push_back(&b_knights_bitboard_);
    _bitboards.push_back(&b_bishops_bitboard);
    _bitboards.push_back(&b_rooks_bitboard);
    _bitboards.push_back(&b_queens_bitboard);
    _bitboards.push_back(&b_king_bitboard);
}

void Bitboards::reset_bitboards() 
{
    for (int i = 0; i < 12; i++)
        *(_bitboards[i]) = 0;
}

} // namespace model