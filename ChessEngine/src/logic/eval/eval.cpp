#include "logic/eval/eval.h"

#include "logic/utils.h"

#include "model/position/position.h"

namespace logic {

static const int PAWN_VALUE = 1000;
static const int KNIGHT_VALUE = 3000;
static const int BISHOP_VALUE = 3000;
static const int ROOK_VALUE = 5000;
static const int QUEEN_VALUE = 9000;

Eval::Eval(const model::Position& pos) 
    : pos_(pos) 
{}

int Eval::evaluate() {
    int score = 0;

    score += PAWN_VALUE * (utils::pop_count(pos_.bbs.get(PieceType::W_PAWN)));
    score -= PAWN_VALUE * (utils::pop_count(pos_.bbs.get(PieceType::B_PAWN)));

    score += KNIGHT_VALUE * (utils::pop_count(pos_.bbs.get(PieceType::W_KNIGHT)));
    score -= KNIGHT_VALUE * (utils::pop_count(pos_.bbs.get(PieceType::B_KNIGHT)));

    score += BISHOP_VALUE * (utils::pop_count(pos_.bbs.get(PieceType::W_BISHOP)));
    score -= BISHOP_VALUE * (utils::pop_count(pos_.bbs.get(PieceType::B_BISHOP)));

    score += ROOK_VALUE * (utils::pop_count(pos_.bbs.get(PieceType::W_ROOK)));
    score -= ROOK_VALUE * (utils::pop_count(pos_.bbs.get(PieceType::B_ROOK)));

    score += QUEEN_VALUE * (utils::pop_count(pos_.bbs.get(PieceType::W_QUEEN)));
    score -= QUEEN_VALUE * (utils::pop_count(pos_.bbs.get(PieceType::B_QUEEN)));

    return pos_.is_w ? score : -score;
}

} // namespace logic