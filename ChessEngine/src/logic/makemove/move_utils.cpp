#include "logic/makemove/move_utils.h"

#include "model/move/move.h"

namespace logic::move_utils {

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

int determine_capture_sq(const model::Move& move, bool is_w)
{
    return move.is_ep_capture() ? is_w ? move.get_to_sq() - 8 
                                       : move.get_to_sq() + 8 
                                : move.get_to_sq();
}
    
} // namespace logic::move_utils