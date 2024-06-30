#include "LookupUpdater.h"

#include "ChessEngine/move/Move.h"

namespace move {
    LookupUpdater::LookupUpdater(board::SquaresLookup& squaresLookup) : _squaresLookup(squaresLookup) {}

    void LookupUpdater::makeCastleMove(bool isWhite, bool isKingSide) {
        if (isWhite) {
            if (isKingSide) {
                _squaresLookup.setPieceTypeAtIndex(3, board::PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(1, board::PieceType::W_KING);
                _squaresLookup.setPieceTypeAtIndex(0, board::PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(2, board::PieceType::W_ROOK);
            } else {
                _squaresLookup.setPieceTypeAtIndex(3, board::PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(5, board::PieceType::W_KING);
                _squaresLookup.setPieceTypeAtIndex(7, board::PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(4, board::PieceType::W_ROOK);
            }
        } else {
            if (isKingSide) {
                _squaresLookup.setPieceTypeAtIndex(59, board::PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(57, board::PieceType::B_KING);
                _squaresLookup.setPieceTypeAtIndex(56, board::PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(58, board::PieceType::B_ROOK);
            } else {
                _squaresLookup.setPieceTypeAtIndex(59, board::PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(61, board::PieceType::B_KING);
                _squaresLookup.setPieceTypeAtIndex(63, board::PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(60, board::PieceType::B_ROOK);
            }
        }
    }

    void LookupUpdater::unmakeCastleMove(bool wasWhite, bool wasKingSide) {
        if (wasWhite) {
            if (wasKingSide) {
                _squaresLookup.setPieceTypeAtIndex(1, board::PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(3, board::PieceType::W_KING);
                _squaresLookup.setPieceTypeAtIndex(2, board::PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(0, board::PieceType::W_ROOK);
            } else {
                _squaresLookup.setPieceTypeAtIndex(5, board::PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(3, board::PieceType::W_KING);
                _squaresLookup.setPieceTypeAtIndex(4, board::PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(7, board::PieceType::W_ROOK);
            }
        } else {
            if (wasKingSide) {
                _squaresLookup.setPieceTypeAtIndex(57, board::PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(59, board::PieceType::B_KING);
                _squaresLookup.setPieceTypeAtIndex(58, board::PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(56, board::PieceType::B_ROOK);
            } else {
                _squaresLookup.setPieceTypeAtIndex(61, board::PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(59, board::PieceType::B_KING);
                _squaresLookup.setPieceTypeAtIndex(60, board::PieceType::EMPTY);
                _squaresLookup.setPieceTypeAtIndex(63, board::PieceType::B_ROOK);
            }
        }
    }

    board::PieceType LookupUpdater::getPromotionPieceType(int promotionFlag, bool isWhite) {
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

}