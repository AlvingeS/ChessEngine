#include "Evaluator.h"

namespace evaluation {
    static const int PAWN_VALUE = 1000;
    static const int KNIGHT_VALUE = 3000;
    static const int BISHOP_VALUE = 3000;
    static const int ROOK_VALUE = 5000;
    static const int QUEEN_VALUE = 9000;

    Evaluator::Evaluator(game::ChessBoard& board) : _board(board) {}

    float Evaluator::evaluate() {
        return 0.0f;
        // int score = 0;

        // score += PAWN_VALUE * (bits::popCount(_board.getBitboard(game::PieceType::W_PAWN)));
        // score -= PAWN_VALUE * (bits::popCount(_board.getBitboard(game::PieceType::B_PAWN)));

        // score += KNIGHT_VALUE * (bits::popCount(_board.getBitboard(game::PieceType::W_KNIGHT)));
        // score -= KNIGHT_VALUE * (bits::popCount(_board.getBitboard(game::PieceType::B_KNIGHT)));

        // score += BISHOP_VALUE * (bits::popCount(_board.getBitboard(game::PieceType::W_BISHOP)));
        // score -= BISHOP_VALUE * (bits::popCount(_board.getBitboard(game::PieceType::B_BISHOP)));

        // score += ROOK_VALUE * (bits::popCount(_board.getBitboard(game::PieceType::W_ROOK)));
        // score -= ROOK_VALUE * (bits::popCount(_board.getBitboard(game::PieceType::B_ROOK)));

        // score += QUEEN_VALUE * (bits::popCount(_board.getBitboard(game::PieceType::W_QUEEN)));
        // score -= QUEEN_VALUE * (bits::popCount(_board.getBitboard(game::PieceType::B_QUEEN)));

        // return static_cast<float>(score);
    }
}