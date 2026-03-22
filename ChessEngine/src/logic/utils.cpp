#include "logic/utils.h"

namespace logic::utils 
{

PieceType get_promotion_piece_type(int promo_flag, bool is_w) 
{
    switch(promo_flag) {
        case model::Move::KNIGHT_PROMO_FLAG:
            return is_w ? PieceType::W_KNIGHT : PieceType::B_KNIGHT;
            break;
        case model::Move::BISHOP_PROMO_FLAG:
            return is_w ? PieceType::W_BISHOP : PieceType::B_BISHOP;
            break;
        case model::Move::ROOK_PROMO_FLAG:
            return is_w ? PieceType::W_ROOK : PieceType::B_ROOK;
            break;
        case model::Move::QUEEN_PROMO_FLAG:
            return is_w ? PieceType::W_QUEEN : PieceType::B_QUEEN;
            break;
        case model::Move::KNIGHT_PROMO_CAPTURE_FLAG:
            return is_w ? PieceType::W_KNIGHT : PieceType::B_KNIGHT;
            break;
        case model::Move::BISHOP_PROMO_CAPTURE_FLAG:
            return is_w ? PieceType::W_BISHOP : PieceType::B_BISHOP;
            break;
        case model::Move::ROOK_PROMO_CAPTURE_FLAG:
            return is_w ? PieceType::W_ROOK : PieceType::B_ROOK;
            break;
        case model::Move::QUEEN_PROMO_CAPTURE_FLAG:
            return is_w ? PieceType::W_QUEEN : PieceType::B_QUEEN;
            break;
    }

    return PieceType::EMPTY;
}

} // namespace logic::utils