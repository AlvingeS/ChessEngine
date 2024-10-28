#include "ChessEngine/src/logic/makemove/MoveUtils.h"

namespace logic {
namespace makemove {

model::PieceType getPromotionPieceType(int promotionFlag, bool isWhite) 
{
    switch(promotionFlag) {
        case model::Move::KNIGHT_PROMO_FLAG:
            return isWhite ?model::PieceType::W_KNIGHT :model::PieceType::B_KNIGHT;
            break;
        case model::Move::BISHOP_PROMO_FLAG:
            return isWhite ?model::PieceType::W_BISHOP :model::PieceType::B_BISHOP;
            break;
        case model::Move::ROOK_PROMO_FLAG:
            return isWhite ?model::PieceType::W_ROOK :model::PieceType::B_ROOK;
            break;
        case model::Move::QUEEN_PROMO_FLAG:
            return isWhite ?model::PieceType::W_QUEEN :model::PieceType::B_QUEEN;
            break;
        case model::Move::KNIGHT_PROMO_CAPTURE_FLAG:
            return isWhite ?model::PieceType::W_KNIGHT :model::PieceType::B_KNIGHT;
            break;
        case model::Move::BISHOP_PROMO_CAPTURE_FLAG:
            return isWhite ?model::PieceType::W_BISHOP :model::PieceType::B_BISHOP;
            break;
        case model::Move::ROOK_PROMO_CAPTURE_FLAG:
            return isWhite ?model::PieceType::W_ROOK :model::PieceType::B_ROOK;
            break;
        case model::Move::QUEEN_PROMO_CAPTURE_FLAG:
            return isWhite ?model::PieceType::W_QUEEN :model::PieceType::B_QUEEN;
            break;
    }

    return model::PieceType::EMPTY;
}

int determineCaptureIndex(const model::Move& move, bool isWhite, int toIndex)
{
    return move.isEpCapture() ? (isWhite ? toIndex - 8 : toIndex + 8) : toIndex;
}
    
} // namespace makemove
} // namespace logic