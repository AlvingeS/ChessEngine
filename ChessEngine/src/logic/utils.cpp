#include "logic/utils.h"

namespace logic::utils 
{

void get_bit_idxs(std::vector<int>& idxs, bitboard bb)
{ 
    idxs.clear();

    while (bb != 0) {
        int idx = __builtin_ctzll(bb);

        idxs.push_back(idx);

        bb &= (bb - 1);
    }
}

model::Piece::Type get_promotion_piece_type(int promo_flag, bool is_w) 
{
    switch(promo_flag) {
        case model::Move::KNIGHT_PROMO_FLAG:
            return is_w ?model::Piece::Type::W_KNIGHT :model::Piece::Type::B_KNIGHT;
            break;
        case model::Move::BISHOP_PROMO_FLAG:
            return is_w ?model::Piece::Type::W_BISHOP :model::Piece::Type::B_BISHOP;
            break;
        case model::Move::ROOK_PROMO_FLAG:
            return is_w ?model::Piece::Type::W_ROOK :model::Piece::Type::B_ROOK;
            break;
        case model::Move::QUEEN_PROMO_FLAG:
            return is_w ?model::Piece::Type::W_QUEEN :model::Piece::Type::B_QUEEN;
            break;
        case model::Move::KNIGHT_PROMO_CAPTURE_FLAG:
            return is_w ?model::Piece::Type::W_KNIGHT :model::Piece::Type::B_KNIGHT;
            break;
        case model::Move::BISHOP_PROMO_CAPTURE_FLAG:
            return is_w ?model::Piece::Type::W_BISHOP :model::Piece::Type::B_BISHOP;
            break;
        case model::Move::ROOK_PROMO_CAPTURE_FLAG:
            return is_w ?model::Piece::Type::W_ROOK :model::Piece::Type::B_ROOK;
            break;
        case model::Move::QUEEN_PROMO_CAPTURE_FLAG:
            return is_w ?model::Piece::Type::W_QUEEN :model::Piece::Type::B_QUEEN;
            break;
    }

    return model::Piece::Type::EMPTY;
}

} // namespace logic::utils