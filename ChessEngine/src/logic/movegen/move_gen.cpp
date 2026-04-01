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

LegalityInfo MoveGen::gen_moves(model::Movelist& movelist, const bool captures_only) const
{
    movelist.reset();

    auto legality_info = check_detection_.generate_legality_info();

    gen_rook_moves(movelist, legality_info, captures_only);
    gen_bishop_moves(movelist, legality_info, captures_only);
    gen_queen_moves(movelist, legality_info, captures_only);
    gen_knight_moves(movelist, legality_info, captures_only);
    gen_pawn_moves(movelist, legality_info, captures_only);
    gen_king_moves(movelist, legality_info, captures_only);
    if (!captures_only) {
        gen_castle_moves(movelist, legality_info);
    }

    movelist.add_null_move();

    return legality_info;
}

void MoveGen::gen_rook_moves(model::Movelist& movelist, const LegalityInfo& legality_info, const bool captures_only) const
{
    rook_gen_.generate(movelist, legality_info, captures_only);
}

void MoveGen::gen_bishop_moves(model::Movelist& movelist, const LegalityInfo& legality_info, const bool captures_only) const
{
    bishop_gen_.generate(movelist, legality_info, captures_only);
}

void MoveGen::gen_queen_moves(model::Movelist& movelist, const LegalityInfo& legality_info, const bool captures_only) const
{
    queen_gen_.generate(movelist, legality_info, captures_only);;
}

void MoveGen::gen_knight_moves(model::Movelist& movelist, const LegalityInfo& legality_info, const bool captures_only) const
{
    knight_gen_.generate(movelist, legality_info, captures_only);
}

void MoveGen::gen_king_moves(model::Movelist& movelist, const LegalityInfo& legality_info, const bool captures_only) const
{
    king_gen_.generate(movelist, legality_info, captures_only);
}

void MoveGen::gen_pawn_moves(model::Movelist& movelist, const LegalityInfo& legality_info, const bool captures_only) const
{
    pawn_gen_.generate(movelist, legality_info, captures_only);
}

void MoveGen::gen_castle_moves(model::Movelist& movelist, const LegalityInfo& legality_info) const
{
    castle_gen_.generate(movelist, legality_info);
}

bool MoveGen::in_check()  const{
    return check_detection_.in_check();
}

} // namespace logic