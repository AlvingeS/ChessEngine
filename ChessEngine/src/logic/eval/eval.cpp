#include "logic/eval/eval.h"

#include "model/position/board.h"

namespace logic {

static const int PAWN_VALUE = 1000;
static const int KNIGHT_VALUE = 3000;
static const int BISHOP_VALUE = 3000;
static const int ROOK_VALUE = 5000;
static const int QUEEN_VALUE = 9000;

Eval::Eval(model::Board& board) 
    : bbs_(board.bbs) 
{}

float Eval::evaluate() {
    return 0.0f;
    // int score = 0;

    // score += PAWN_VALUE * (bits::pop_count(board_.get_bb_from_piece_type(model::Piece::Type::W_PAWN)));
    // score -= PAWN_VALUE * (bits::pop_count(board_.get_bb_from_piece_type(model::Piece::Type::B_PAWN)));

    // score += KNIGHT_VALUE * (bits::pop_count(board_.get_bb_from_piece_type(model::Piece::Type::W_KNIGHT)));
    // score -= KNIGHT_VALUE * (bits::pop_count(board_.get_bb_from_piece_type(model::Piece::Type::B_KNIGHT)));

    // score += BISHOP_VALUE * (bits::pop_count(board_.get_bb_from_piece_type(model::Piece::Type::W_BISHOP)));
    // score -= BISHOP_VALUE * (bits::pop_count(board_.get_bb_from_piece_type(model::Piece::Type::B_BISHOP)));

    // score += ROOK_VALUE * (bits::pop_count(board_.get_bb_from_piece_type(model::Piece::Type::W_ROOK)));
    // score -= ROOK_VALUE * (bits::pop_count(board_.get_bb_from_piece_type(model::Piece::Type::B_ROOK)));

    // score += QUEEN_VALUE * (bits::pop_count(board_.get_bb_from_piece_type(model::Piece::Type::W_QUEEN)));
    // score -= QUEEN_VALUE * (bits::pop_count(board_.get_bb_from_piece_type(model::Piece::Type::B_QUEEN)));

    // return static_cast<float>(score);
}

} // namespace logic