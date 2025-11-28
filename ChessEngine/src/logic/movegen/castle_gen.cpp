#include "logic/movegen/castle_gen.h"

#include "model/position/position.h"

#include "logic/utils.h"
#include "logic/movegen/check_detection.h"
#include "logic/masks.h"

#include "logic/makemove/move_maker.h"
#include "logic/makemove/move_retractor.h"

#include "model/move/movelist.h"
#include "model/move/move.h"

namespace logic {

CastleGen::CastleGen(
    const model::Position& pos,
    logic::MoveMaker& move_maker, 
    logic::MoveRetractor& move_retractor, 
    CheckDetection* check_detection)
    : pos_(pos)
    , move_maker_(move_maker)
    , move_retractor_(move_retractor)
    , check_detection_(check_detection) 
{}

void CastleGen::generate(bool is_w, model::Movelist& movelist) 
{
    if (pos_.c_rights == 0) {
        return;
    }
    
    // We can generate up to two castle moves depedent on the side and
    // respective castle rights
    if (is_w) {
        if (pos_.c_rights & masks::W_KSIDE_CASTLE_RIGHTS_MASK)
            gen_single_castle_move(is_w, true, movelist);

        if (pos_.c_rights & masks::W_QSIDE_CASTLE_RIGHTS_MASK)
            gen_single_castle_move(is_w, false, movelist);
    } else {
        if (pos_.c_rights & masks::B_KSIDE_CASTLE_RIGHTS_MASK)
            gen_single_castle_move(is_w, true, movelist);

        if (pos_.c_rights & masks::B_QSIDE_CASTLE_RIGHTS_MASK)
            gen_single_castle_move(is_w, false, movelist);
    }
}

bool CastleGen::king_and_rook_on_castle_squares(bool is_w, bool is_kside) const
{
    bool king_bit_enabled = is_w ? (pos_.bbs.get_w_king_bb() & (1ULL << 3)) != 0
                                 : (pos_.bbs.get_b_king_bb() & (1ULL << 59)) != 0;
    
    if (!king_bit_enabled)
        return false;

    // Since we know that the king is present, we can return if the rook is present or not
    return is_w ? (is_kside ? (pos_.bbs.get_w_rooks_bb() & (1ULL << 0)) != 0
                            : (pos_.bbs.get_w_rooks_bb() & (1ULL << 7)) != 0)
                : (is_kside ? (pos_.bbs.get_b_rooks_bb() & (1ULL << 56)) != 0
                            : (pos_.bbs.get_b_rooks_bb() & (1ULL << 63)) != 0);
}

void CastleGen::make_temporary_king_move(bool is_w, bool is_kside)
{
    move_maker_.make_temporary_king_move(is_w, is_kside);
}

void CastleGen::revert_temporary_king_move(bool is_w, bool is_kside)
{
    move_retractor_.revert_temporary_king_move(is_w, is_kside);
}

void CastleGen::gen_single_castle_move(
    bool is_w,
    bool is_kside,
    model::Movelist& movelist)
{                                                  
    // Check that there are no pieces between the king and rook
    bitmask space_between_castlers_mask = is_w ? (is_kside ? masks::W_KSIDE_SPACE_BETWEEN_KING_AND_ROOK_MASK 
                                                           : masks::W_QSIDE_SPACE_BETWEEN_KING_AND_ROOK_MASK)
                                               : (is_kside ? masks::B_KSIDE_SPACE_BETWEEN_KING_AND_ROOK_MASK
                                                           : masks::B_QSIDE_SPACE_BETWEEN_KING_AND_ROOK_MASK);
    
    if ((space_between_castlers_mask & pos_.occ_masks.get_occupied_squares_mask()) != 0)
        return;

    // Check that the king and rook are on the correct squares
    if (!king_and_rook_on_castle_squares(is_w, is_kside))
        return;

    // Check that we are not currently in check
    if (check_detection_->in_check(is_w))
        return;

    // Move king one square towards the rook, check that the king is not in check
    make_temporary_king_move(is_w, is_kside);
    
    if (check_detection_->in_check(is_w)) {
        revert_temporary_king_move(is_w, is_kside);
        return;
    }
    
    revert_temporary_king_move(is_w, is_kside);

    int move_flag = is_kside ? model::Move::KING_CASTLE_FLAG 
                             : model::Move::QUEEN_CASTLE_FLAG;

    movelist.add_move(model::Move(0, 0, move_flag));
}

} // namespace logic