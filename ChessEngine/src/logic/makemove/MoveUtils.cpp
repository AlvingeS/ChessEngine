#include "logic/makemove/MoveUtils.h"

#include "model/move/Move.h"

namespace logic {

model::Piece::Type MoveUtils::getPromotionPieceType(int promotionFlag, bool isWhite) 
{
    switch(promotionFlag) {
        case model::Move::KNIGHT_PROMO_FLAG:
            return isWhite ?model::Piece::Type::W_KNIGHT :model::Piece::Type::B_KNIGHT;
            break;
        case model::Move::BISHOP_PROMO_FLAG:
            return isWhite ?model::Piece::Type::W_BISHOP :model::Piece::Type::B_BISHOP;
            break;
        case model::Move::ROOK_PROMO_FLAG:
            return isWhite ?model::Piece::Type::W_ROOK :model::Piece::Type::B_ROOK;
            break;
        case model::Move::QUEEN_PROMO_FLAG:
            return isWhite ?model::Piece::Type::W_QUEEN :model::Piece::Type::B_QUEEN;
            break;
        case model::Move::KNIGHT_PROMO_CAPTURE_FLAG:
            return isWhite ?model::Piece::Type::W_KNIGHT :model::Piece::Type::B_KNIGHT;
            break;
        case model::Move::BISHOP_PROMO_CAPTURE_FLAG:
            return isWhite ?model::Piece::Type::W_BISHOP :model::Piece::Type::B_BISHOP;
            break;
        case model::Move::ROOK_PROMO_CAPTURE_FLAG:
            return isWhite ?model::Piece::Type::W_ROOK :model::Piece::Type::B_ROOK;
            break;
        case model::Move::QUEEN_PROMO_CAPTURE_FLAG:
            return isWhite ?model::Piece::Type::W_QUEEN :model::Piece::Type::B_QUEEN;
            break;
    }

    return model::Piece::Type::EMPTY;
}

int MoveUtils::determineCaptureIndex(const model::Move& move, bool isWhite, int toIndex)
{
    return move.isEpCapture() ? isWhite ? toIndex - 8 
                                        : toIndex + 8 
                              : toIndex;
}
    
} // namespace logic