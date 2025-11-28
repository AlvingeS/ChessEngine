#include "logic/movegen/move_gen.h"

#include "logic/makemove/move_maker.h"
#include "logic/makemove/move_retractor.h"

#include "model/position/position.h"
#include "model/move/movelist.h"

#include <iostream>

namespace logic {

MoveGen::MoveGen(
    const model::Position& pos,
    logic::MoveMaker& move_maker,
    logic::MoveRetractor& move_retractor)
    : pos_(pos)
    , move_maker_(move_maker)
    , move_retractor_(move_retractor)
    , check_detection_(pos)
    , rook_gen_(pos)
    , bishop_gen_(pos)
    , queen_gen_(pos)
    , knight_gen_(pos)
    , king_gen_(pos)
    , pawn_gen_(pos)
    , castle_gen_(pos, move_maker, move_retractor, &check_detection_)
{}

void MoveGen::gen_moves(bool is_w, model::Movelist& movelist)
{
    movelist.reset();
    gen_rook_moves(is_w, movelist);
    gen_bishop_moves(is_w, movelist);
    gen_queen_moves(is_w, movelist);
    gen_knight_moves(is_w, movelist);
    gen_king_moves(is_w, movelist);
    gen_pawn_moves(is_w, movelist);
    gen_castle_moves(is_w, movelist);
    movelist.add_null_move(); // Add a null move to the end of the move list
}

void MoveGen::gen_rook_moves(bool is_w, model::Movelist& movelist)
{
    rook_gen_.generate(is_w, movelist);
}

void MoveGen::gen_bishop_moves(bool is_w, model::Movelist& movelist)
{
    bishop_gen_.generate(is_w, movelist);
}

void MoveGen::gen_queen_moves(bool is_w, model::Movelist& movelist)
{
    queen_gen_.generate(is_w, movelist);;
}

void MoveGen::gen_knight_moves(bool is_w, model::Movelist& movelist)
{
    knight_gen_.generate(is_w, movelist);
}

void MoveGen::gen_king_moves(bool is_w, model::Movelist& movelist)
{
    king_gen_.generate(is_w, movelist);
}

void MoveGen::gen_pawn_moves(bool is_w, model::Movelist& movelist)
{
    pawn_gen_.generate(is_w, movelist);
}

void MoveGen::gen_castle_moves(bool is_w, model::Movelist& movelist)
{
    castle_gen_.generate(is_w, movelist);
}

bool MoveGen::in_check(bool is_w) {
    return check_detection_.in_check(is_w);
}

} // namespace logic