#include "logic/eval/eval.h"

#include "logic/utils.h"

#include "model/position/position.h"

namespace logic {

static constexpr int PAWN_VALUE   = 100;
static constexpr int KNIGHT_VALUE = 320;
static constexpr int BISHOP_VALUE = 330;
static constexpr int ROOK_VALUE   = 500;
static constexpr int QUEEN_VALUE  = 900;

static constexpr int PAWN_PHASE = 0;
static constexpr int KNIGHT_PHASE = 1;
static constexpr int BISHOP_PHASE = 1;
static constexpr int ROOK_PHASE = 2;
static constexpr int QUEEN_PHASE = 4;
static constexpr int START_PHASE = PAWN_PHASE*16 + KNIGHT_PHASE*4 + BISHOP_PHASE*4 + ROOK_PHASE*4 + QUEEN_PHASE*2;

// Piece-square tables from white's perspective
// Index 0 = a1, index 7 = h1, index 56 = a8, index 63 = h8
// Values are bonuses/penalties in ~centipawns

static constexpr int PAWN_PST[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
    5,  5, 10, 25, 25, 10,  5,  5,
    0,  0,  0, 20, 20,  0,  0,  0,
    5, -5,-10,  0,  0,-10, -5,  5,
    5, 10, 10,-20,-20, 10, 10,  5,
    0,  0,  0,  0,  0,  0,  0,  0
};

static constexpr int KNIGHT_PST[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50,
};

static constexpr int BISHOP_PST[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20,
};

static constexpr int ROOK_PST[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
    5, 10, 10, 10, 10, 10, 10,  5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    0,  0,  0,  5,  5,  0,  0,  0
};

static constexpr int QUEEN_PST[64] = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -5,  0,  5,  5,  5,  5,  0, -5,
    0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};

static constexpr int KING_PST_MIDGAME[64] = {
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    20, 20,  0,  0,  0,  0, 20, 20,
    20, 30, 10,  0,  0, 10, 30, 20
};

static constexpr int KING_PST_ENDGAME[64] = {
    -50,-40,-30,-20,-20,-30,-40,-50,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-30,  0,  0,  0,  0,-30,-30,
    -50,-30,-30,-30,-30,-30,-30,-50
};

// Mirror a square vertically for black pieces: rank 0 <-> rank 7
static constexpr sq_t mirror_sq(sq_t sq) {
    return sq ^ 56;
}

// Sum PST values for all pieces of a given type
static int pst_score(const model::Bitboards& bbs, PieceType pt, const int* table) {
    int score = 0;
    bitmask bb = bbs.get(pt);
    while (bb) {
        sq_t sq = utils::lsb_idx(bb);
        score += table[mirror_sq(sq)];
        bb &= bb - 1;
    }
    return score;
}

// Same but mirror the square lookup (for black pieces)
static int pst_score_mirrored(const model::Bitboards& bbs, PieceType pt, const int* table) {
    int score = 0;
    bitmask bb = bbs.get(pt);
    while (bb) {
        sq_t sq = utils::lsb_idx(bb);
        score += table[sq];
        bb &= bb - 1;
    }
    return score;
}

Eval::Eval(const model::Position& pos) 
    : pos_(pos) 
{}

int Eval::get_phase() const
{
    int phase = START_PHASE;

    phase -= utils::pop_count(pos_.bbs.get(PieceType::W_PAWN))   * PAWN_PHASE;
    phase -= utils::pop_count(pos_.bbs.get(PieceType::W_KNIGHT)) * KNIGHT_PHASE;
    phase -= utils::pop_count(pos_.bbs.get(PieceType::W_BISHOP)) * BISHOP_PHASE;
    phase -= utils::pop_count(pos_.bbs.get(PieceType::W_ROOK))   * ROOK_PHASE;
    phase -= utils::pop_count(pos_.bbs.get(PieceType::W_QUEEN))  * QUEEN_PHASE;
    phase -= utils::pop_count(pos_.bbs.get(PieceType::B_PAWN))   * PAWN_PHASE;
    phase -= utils::pop_count(pos_.bbs.get(PieceType::B_KNIGHT)) * KNIGHT_PHASE;
    phase -= utils::pop_count(pos_.bbs.get(PieceType::B_BISHOP)) * BISHOP_PHASE;
    phase -= utils::pop_count(pos_.bbs.get(PieceType::B_ROOK))   * ROOK_PHASE;
    phase -= utils::pop_count(pos_.bbs.get(PieceType::B_QUEEN))  * QUEEN_PHASE;

    return phase;
}

eval_t Eval::get_tapered_king_eval(sq_t sq_idx, bool mirror) const
{
    int phase = get_phase();

    if (mirror) {
        return (KING_PST_MIDGAME[sq_idx] * (START_PHASE - phase) + KING_PST_ENDGAME[sq_idx] * phase) / START_PHASE;
    } else {
        return (KING_PST_MIDGAME[mirror_sq(sq_idx)] * (START_PHASE - phase) + KING_PST_ENDGAME[mirror_sq(sq_idx)] * phase) / START_PHASE;
    }
}

eval_t Eval::evaluate() const {
    eval_t score = 0;

    // Material
    score += PAWN_VALUE   * utils::pop_count(pos_.bbs.get(PieceType::W_PAWN));
    score -= PAWN_VALUE   * utils::pop_count(pos_.bbs.get(PieceType::B_PAWN));
    score += KNIGHT_VALUE * utils::pop_count(pos_.bbs.get(PieceType::W_KNIGHT));
    score -= KNIGHT_VALUE * utils::pop_count(pos_.bbs.get(PieceType::B_KNIGHT));
    score += BISHOP_VALUE * utils::pop_count(pos_.bbs.get(PieceType::W_BISHOP));
    score -= BISHOP_VALUE * utils::pop_count(pos_.bbs.get(PieceType::B_BISHOP));
    score += ROOK_VALUE   * utils::pop_count(pos_.bbs.get(PieceType::W_ROOK));
    score -= ROOK_VALUE   * utils::pop_count(pos_.bbs.get(PieceType::B_ROOK));
    score += QUEEN_VALUE  * utils::pop_count(pos_.bbs.get(PieceType::W_QUEEN));
    score -= QUEEN_VALUE  * utils::pop_count(pos_.bbs.get(PieceType::B_QUEEN));

    // Piece-square bonuses
    score += pst_score(pos_.bbs, PieceType::W_PAWN,   PAWN_PST);
    score -= pst_score_mirrored(pos_.bbs, PieceType::B_PAWN,   PAWN_PST);

    score += pst_score(pos_.bbs, PieceType::W_KNIGHT, KNIGHT_PST);
    score -= pst_score_mirrored(pos_.bbs, PieceType::B_KNIGHT, KNIGHT_PST);

    score += pst_score(pos_.bbs, PieceType::W_BISHOP, BISHOP_PST);
    score -= pst_score_mirrored(pos_.bbs, PieceType::B_BISHOP, BISHOP_PST);

    score += pst_score(pos_.bbs, PieceType::W_ROOK,   ROOK_PST);
    score -= pst_score_mirrored(pos_.bbs, PieceType::B_ROOK,   ROOK_PST);

    score += pst_score(pos_.bbs, PieceType::W_QUEEN,  QUEEN_PST);
    score -= pst_score_mirrored(pos_.bbs, PieceType::B_QUEEN,  QUEEN_PST);

    score += get_tapered_king_eval(logic::utils::lsb_idx(pos_.bbs.get(PieceType::W_KING)), false);
    score += get_tapered_king_eval(logic::utils::lsb_idx(pos_.bbs.get(PieceType::B_KING)), true);

    return pos_.is_w ? score : -score;
}

} // namespace logic