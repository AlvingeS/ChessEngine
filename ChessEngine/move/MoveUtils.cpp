#include "ChessEngine/move/MoveUtils.h"

namespace move {
namespace moveutils {

board::PieceType getPromotionPieceType(int promotionFlag, bool isWhite) 
{
    switch(promotionFlag) {
        case Move::KNIGHT_PROMO_FLAG:
            return isWhite ? board::PieceType::W_KNIGHT : board::PieceType::B_KNIGHT;
            break;
        case Move::BISHOP_PROMO_FLAG:
            return isWhite ? board::PieceType::W_BISHOP : board::PieceType::B_BISHOP;
            break;
        case Move::ROOK_PROMO_FLAG:
            return isWhite ? board::PieceType::W_ROOK : board::PieceType::B_ROOK;
            break;
        case Move::QUEEN_PROMO_FLAG:
            return isWhite ? board::PieceType::W_QUEEN : board::PieceType::B_QUEEN;
            break;
        case Move::KNIGHT_PROMO_CAPTURE_FLAG:
            return isWhite ? board::PieceType::W_KNIGHT : board::PieceType::B_KNIGHT;
            break;
        case Move::BISHOP_PROMO_CAPTURE_FLAG:
            return isWhite ? board::PieceType::W_BISHOP : board::PieceType::B_BISHOP;
            break;
        case Move::ROOK_PROMO_CAPTURE_FLAG:
            return isWhite ? board::PieceType::W_ROOK : board::PieceType::B_ROOK;
            break;
        case Move::QUEEN_PROMO_CAPTURE_FLAG:
            return isWhite ? board::PieceType::W_QUEEN : board::PieceType::B_QUEEN;
            break;
    }

    return board::PieceType::EMPTY;
}

int determineCaptureIndex(const Move& move, bool isWhite, int toIndex)
{
    return move.isEpCapture() ? (isWhite ? toIndex - 8 : toIndex + 8) : toIndex;
}
    
} // namespace utils
} // namespace move