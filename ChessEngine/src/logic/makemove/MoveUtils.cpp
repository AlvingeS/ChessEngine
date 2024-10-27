#include "ChessEngine/src/logic/makemove/MoveUtils.h"

namespace logic {
namespace makemove {

representation::board::PieceType getPromotionPieceType(int promotionFlag, bool isWhite) 
{
    switch(promotionFlag) {
        case representation::move::Move::KNIGHT_PROMO_FLAG:
            return isWhite ?representation::board::PieceType::W_KNIGHT :representation::board::PieceType::B_KNIGHT;
            break;
        case representation::move::Move::BISHOP_PROMO_FLAG:
            return isWhite ?representation::board::PieceType::W_BISHOP :representation::board::PieceType::B_BISHOP;
            break;
        case representation::move::Move::ROOK_PROMO_FLAG:
            return isWhite ?representation::board::PieceType::W_ROOK :representation::board::PieceType::B_ROOK;
            break;
        case representation::move::Move::QUEEN_PROMO_FLAG:
            return isWhite ?representation::board::PieceType::W_QUEEN :representation::board::PieceType::B_QUEEN;
            break;
        case representation::move::Move::KNIGHT_PROMO_CAPTURE_FLAG:
            return isWhite ?representation::board::PieceType::W_KNIGHT :representation::board::PieceType::B_KNIGHT;
            break;
        case representation::move::Move::BISHOP_PROMO_CAPTURE_FLAG:
            return isWhite ?representation::board::PieceType::W_BISHOP :representation::board::PieceType::B_BISHOP;
            break;
        case representation::move::Move::ROOK_PROMO_CAPTURE_FLAG:
            return isWhite ?representation::board::PieceType::W_ROOK :representation::board::PieceType::B_ROOK;
            break;
        case representation::move::Move::QUEEN_PROMO_CAPTURE_FLAG:
            return isWhite ?representation::board::PieceType::W_QUEEN :representation::board::PieceType::B_QUEEN;
            break;
    }

    return representation::board::PieceType::EMPTY;
}

int determineCaptureIndex(const representation::move::Move& move, bool isWhite, int toIndex)
{
    return move.isEpCapture() ? (isWhite ? toIndex - 8 : toIndex + 8) : toIndex;
}
    
} // namespace makemove
} // namespace logic