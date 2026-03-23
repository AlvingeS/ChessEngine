#include "logic/movegen/move_gen.h"

#include "logic/movegen/check_detection.h"
#include "logic/makemove/move_maker.h"
#include "logic/makemove/move_retractor.h"

#include "model/position/position.h"
#include "model/move/movelist.h"

#include <iostream>

namespace logic {

MoveGen::MoveGen(const model::Position& pos)
    : pos_(pos)
    , check_detection_(pos)
    , rook_gen_(pos)
    , bishop_gen_(pos)
    , queen_gen_(pos)
    , knight_gen_(pos)
    , king_gen_(pos)
    , pawn_gen_(pos)
    , castle_gen_(pos)
{}

LegalityInfo MoveGen::gen_moves(model::Movelist& movelist)
{
    movelist.reset();

    auto legality_info = check_detection_.generate_legality_info();

    gen_rook_moves(movelist, legality_info);
    gen_bishop_moves(movelist, legality_info);
    gen_queen_moves(movelist, legality_info);
    gen_knight_moves(movelist, legality_info);
    gen_king_moves(movelist, legality_info);
    gen_pawn_moves(movelist, legality_info);
    gen_castle_moves(movelist, legality_info);

    movelist.add_null_move();

    return legality_info;
}

void MoveGen::gen_rook_moves(model::Movelist& movelist, const LegalityInfo& legality_info)
{
    rook_gen_.generate(movelist, legality_info);
}

void MoveGen::gen_bishop_moves(model::Movelist& movelist, const LegalityInfo& legality_info)
{
    bishop_gen_.generate(movelist, legality_info);
}

void MoveGen::gen_queen_moves(model::Movelist& movelist, const LegalityInfo& legality_info)
{
    queen_gen_.generate(movelist, legality_info);;
}

void MoveGen::gen_knight_moves(model::Movelist& movelist, const LegalityInfo& legality_info)
{
    knight_gen_.generate(movelist, legality_info);
}

void MoveGen::gen_king_moves(model::Movelist& movelist, const LegalityInfo& legality_info)
{
    king_gen_.generate(movelist, legality_info);
}

void MoveGen::gen_pawn_moves(model::Movelist& movelist, const LegalityInfo& legality_info)
{
    pawn_gen_.generate(movelist, legality_info);
}

void MoveGen::gen_castle_moves(model::Movelist& movelist, const LegalityInfo& legality_info)
{
    castle_gen_.generate(movelist, legality_info);
}

bool MoveGen::in_check() {
    return check_detection_.in_check();
}

} // namespace logic