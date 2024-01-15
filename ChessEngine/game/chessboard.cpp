#include "chessboard.h"

namespace game {

    ChessBoard::ChessBoard() {
            bitboards[PieceType::W_PAWN] = 0x00FF000000000000ULL;
            bitboards[PieceType::W_KNIGHT] = 0x4200000000000000ULL;
            bitboards[PieceType::W_BISHOP] = 0x2400000000000000ULL;
            bitboards[PieceType::W_ROOK] = 0x8100000000000000ULL;
            bitboards[PieceType::W_QUEEN] = 0x0800000000000000ULL;
            bitboards[PieceType::W_KING] = 0x1000000000000000ULL;

            bitboards[PieceType::B_PAWN] = 0x000000000000FF00ULL;
            bitboards[PieceType::B_KNIGHT] = 0x0000000000000042ULL;
            bitboards[PieceType::B_BISHOP] = 0x0000000000000024ULL;
            bitboards[PieceType::B_ROOK] = 0x0000000000000081ULL;
            bitboards[PieceType::B_QUEEN] = 0x0000000000000008ULL;
            bitboards[PieceType::B_KING] = 0x0000000000000010ULL;
    }

}