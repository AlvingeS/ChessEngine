#include "logic/movegen/move_gen.h"

#include "logic/makemove/move_maker.h"
#include "logic/makemove/move_retractor.h"

#include "model/position/board.h"
#include "model/move/movelist.h"

#include "engine/pickmove/search_memory.h"

#include <iostream>

namespace logic {

MoveGen::MoveGen(
    model::Board& board,
    logic::MoveMaker& move_maker,
    logic::MoveRetractor& move_retractor)
    : bitboards_(board.bitboards)
    , occupancy_masks_(board.occupancy_masks)
    , move_maker_(move_maker)
    , move_retractor_(move_retractor)
    , check_detection_(board)
    , rook_gen_(board)
    , bishop_gen_(board)
    , queen_gen_(board)
    , knight_gen_(board)
    , king_gen_(board)
    , pawn_gen_(board)
    , castle_gen_(board, move_maker, move_retractor, &check_detection_)
{}

void MoveGen::gen_moves(
    bool is_w,
    model::Movelist& movelist,
    bitmask ep_target_mask,
    unsigned char castle_rights)
{
    movelist.reset();
    gen_rook_moves(is_w, movelist);
    gen_bishop_moves(is_w, movelist);
    gen_queen_moves(is_w, movelist);
    gen_knight_moves(is_w, movelist);
    gen_king_moves(is_w, movelist);
    gen_pawn_moves(is_w, movelist, ep_target_mask);
    gen_castle_moves(is_w, movelist, castle_rights);
    movelist.add_null_move(); // Add a null move to the end of the move list
}

void MoveGen::gen_rook_moves(
    bool is_w,
    model::Movelist& movelist)
{
    rook_gen_.generate(is_w, movelist);
}

void MoveGen::gen_bishop_moves(
    bool is_w,
    model::Movelist& movelist)
{
    bishop_gen_.generate(is_w, movelist);
}

void MoveGen::gen_queen_moves(
    bool is_w,
    model::Movelist& movelist)
{
    queen_gen_.generate(is_w, movelist);;
}

void MoveGen::gen_knight_moves(
    bool is_w,
    model::Movelist& movelist)
{
    knight_gen_.generate(is_w, movelist);
}

void MoveGen::gen_king_moves(
    bool is_w,
    model::Movelist& movelist)
{
    king_gen_.generate(is_w, movelist);
}

void MoveGen::gen_pawn_moves(
    bool is_w,
    model::Movelist& movelist,
    bitmask ep_target_mask)
{
    pawn_gen_.generate(is_w, movelist, ep_target_mask);
}

void MoveGen::gen_castle_moves(
    bool is_w,
    model::Movelist& movelist,
    unsigned char castle_rights)
{
    castle_gen_.generate(is_w, movelist, castle_rights);
}

bool MoveGen::in_check(bool is_w) {
    return check_detection_.in_check(is_w);
}

} // namespace logic