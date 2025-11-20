#include "logic/movegen/castle_gen.h"

#include "model/masks.h"
#include "model/position/board.h"

#include "logic/movegen/utils/chess_utils.h"
#include "logic/movegen/check_detection.h"

#include "logic/makemove/move_maker.h"
#include "logic/makemove/move_retractor.h"

#include "model/move/movelist.h"
#include "model/move/move.h"

namespace logic {

CastleGenerator::CastleGenerator(
    model::Board& board,
    logic::MoveMaker& move_maker, 
    logic::MoveRetractor& move_retractor, 
    CheckDetection* check_detection)
    : bitboards_(board.bitboards)
    , occupancy_masks_(board.occupancy_masks)
    , move_maker_(move_maker)
    , move_retractor_(move_retractor)
    , check_detection_(check_detection) 
{
    w_kside_castle_mask_ = masks::W_KSIDE_CASTLE_MASK;
    w_qside_castle_mask_ = masks::W_QSIDE_CASTLE_MASK;
    b_kside_castle_mask_ = masks::B_KSIDE_CASTLE_MASK;
    b_qside_castle_mask_ = masks::B_QSIDE_CASTLE_MASK;
}

void CastleGenerator::generate(
    bool is_w,
    model::Movelist& movelist,
    unsigned char castle_rights) 
{
    if (castle_rights == 0) {
        return;
    }
    
    if (is_w) {
        if (castle_rights & 0b0001)
            gen_single_castle_move(is_w, true, movelist);

        if (castle_rights & 0b0010)
            gen_single_castle_move(is_w, false, movelist);
    } else {
        if (castle_rights & 0b0100)
            gen_single_castle_move(is_w, true, movelist);

        if (castle_rights & 0b1000)
            gen_single_castle_move(is_w, false, movelist);
    }
}

bool CastleGenerator::king_and_rook_on_castle_squares(bool is_w, bool is_kside) const
{
    bool king_bit_enabled = is_w ? (bitboards_.get_w_king_bb() & (1ULL << 3)) != 0
                                  : (bitboards_.get_b_king_bb() & (1ULL << 59)) != 0;
    
    if (!king_bit_enabled)
        return false;

    // Since we know that the king is present, we can return if the rook is present or not
    return is_w ? (is_kside ? (bitboards_.get_w_rooks_bb() & (1ULL << 0)) != 0
                                 : (bitboards_.get_w_rooks_bb() & (1ULL << 7)) != 0)
                   : (is_kside ? (bitboards_.get_b_rooks_bb() & (1ULL << 56)) != 0
                                 : (bitboards_.get_b_rooks_bb() & (1ULL << 63)) != 0);
}

void CastleGenerator::make_temporary_king_move(bool is_w, bool is_kside)
{
    move_maker_.make_temporary_king_move(is_w, is_kside);
}

void CastleGenerator::revert_temporary_king_move(bool is_w, bool is_kside)
{
    move_retractor_.revert_temporary_king_move(is_w, is_kside);
}

void CastleGenerator::gen_single_castle_move(
    bool is_w,
    bool is_kside,
    model::Movelist& movelist)
{                                                  
    // Check that there are no pieces between the king and rook
    bitmask space_between_castlers_mask = is_w ? (is_kside ? w_kside_castle_mask_ 
                                                                : w_qside_castle_mask_)
                                                  : (is_kside ? b_kside_castle_mask_
                                                                : b_qside_castle_mask_);
    
    if ((space_between_castlers_mask & occupancy_masks_.get_occupied_squares_mask()) != 0)
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