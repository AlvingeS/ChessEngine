#include "model/position/bitboards.h"

namespace model {

Bitboards::Bitboards() 
{
    bbs_[PieceType::W_PAWN]   = 0x000000000000FF00ULL;
    bbs_[PieceType::W_KNIGHT] = 0x0000000000000042ULL;
    bbs_[PieceType::W_BISHOP] = 0x0000000000000024ULL;
    bbs_[PieceType::W_ROOK]   = 0x0000000000000081ULL;
    bbs_[PieceType::W_QUEEN]  = 0x0000000000000008ULL;
    bbs_[PieceType::W_KING]   = 0x0000000000000010ULL;

    bbs_[PieceType::B_PAWN]   = 0x00FF000000000000ULL;
    bbs_[PieceType::B_KNIGHT] = 0x4200000000000000ULL;
    bbs_[PieceType::B_BISHOP] = 0x2400000000000000ULL;
    bbs_[PieceType::B_ROOK]   = 0x8100000000000000ULL;
    bbs_[PieceType::B_QUEEN]  = 0x0800000000000000ULL;
    bbs_[PieceType::B_KING]   = 0x1000000000000000ULL;

    // w_    = 0x000000000000FFFF;
    // b_    = 0xFFFF000000000000;
}

void Bitboards::reset_bitboards() 
{
    for (int i = 0; i < 12; i++) {
        bbs_[i] = 0ULL;
    }
}

PieceType Bitboards::get_piece_type_at(sq_t sq, bool is_w) const {
    //assert(sq >= 0 && sq <= 63);

    if (get_empty_bit(sq)) {
        return PieceType::EMPTY;
    }
    
    if (is_w) {
        for (int i = 0; i <= PieceType::W_KING; i++) {
            if (get_bit_at(sq, static_cast<PieceType>(i))) {
                return static_cast<PieceType>(i);
            }
        }
    } else {
        for (int i = PieceType::B_PAWN; i <= PieceType::B_KING; i++) {
            if (get_bit_at(sq, static_cast<PieceType>(i))) {
                return static_cast<PieceType>(i);
            }
        }
    }

    //assert(false);
    return PieceType::EMPTY;
}

PieceType Bitboards::get_piece_type_at(sq_t sq) const {
    //assert(sq >= 0 && sq <= 63);

    if (get_empty_bit(sq)) {
        return PieceType::EMPTY;
    }
    
    for (int i = 0; i <= PieceType::W_KING; i++) {
        if (get_bit_at(sq, static_cast<PieceType>(i))) {
            return static_cast<PieceType>(i);
        }
    }

    for (int i = PieceType::B_PAWN; i <= PieceType::B_KING; i++) {
        if (get_bit_at(sq, static_cast<PieceType>(i))) {
            return static_cast<PieceType>(i);
        }
    }

    //assert(false);
    return PieceType::EMPTY;
}

} // namespace model