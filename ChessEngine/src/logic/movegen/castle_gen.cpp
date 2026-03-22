#include "logic/movegen/castle_gen.h"

#include "model/position/position.h"

#include "logic/utils.h"
#include "logic/masks.h"

#include "logic/movegen/check_detection.h"
#include "logic/makemove/move_maker.h"
#include "logic/makemove/move_retractor.h"

#include "model/move/movelist.h"
#include "model/move/move.h"
#include "model/constants.h"

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

void CastleGen::generate(model::Movelist& movelist, const LegalityInfo& legality_info) 
{
    if (legality_info.in_check()) {
        return;
    }

    if (pos_.c_rights == 0) {
        return;
    }
    
    // We can generate up to two castle moves depedent on the side and
    // respective castle rights
    if (pos_.is_w) {
        if (pos_.c_rights & masks::W_KSIDE_CASTLE_RIGHTS_MASK)
            gen_single_castle_move(true, movelist, legality_info);

        if (pos_.c_rights & masks::W_QSIDE_CASTLE_RIGHTS_MASK)
            gen_single_castle_move(false, movelist, legality_info);
    } else {
        if (pos_.c_rights & masks::B_KSIDE_CASTLE_RIGHTS_MASK)
            gen_single_castle_move(true, movelist, legality_info);

        if (pos_.c_rights & masks::B_QSIDE_CASTLE_RIGHTS_MASK)
            gen_single_castle_move(false, movelist, legality_info);
    }
}

bool CastleGen::king_and_rook_on_castle_squares(bool is_kside) const
{
    bool king_bit_enabled = pos_.is_w ? (pos_.bbs.get_w_king_bb() & (1ULL << constants::W_KING_START_SQ)) != 0
                                      : (pos_.bbs.get_b_king_bb() & (1ULL << constants::B_KING_START_SQ)) != 0;
    
    if (!king_bit_enabled)
        return false;

    // Since we know that the king is present, we can return if the rook is present or not
    return pos_.is_w ? (is_kside ? (pos_.bbs.get_w_rooks_bb() & (1ULL << constants::W_KSIDE_ROOK_START_SQ)) != 0
                                 : (pos_.bbs.get_w_rooks_bb() & (1ULL << constants::W_QSIDE_ROOK_START_SQ)) != 0)
                     : (is_kside ? (pos_.bbs.get_b_rooks_bb() & (1ULL << constants::B_KSIDE_ROOK_START_SQ)) != 0
                                 : (pos_.bbs.get_b_rooks_bb() & (1ULL << constants::B_QSIDE_ROOK_START_SQ)) != 0);
}

void CastleGen::gen_single_castle_move(
    bool is_kside,
    model::Movelist& movelist,
    const LegalityInfo& legality_info)
{                                                  
    // Check that there are no pieces between the king and rook
    bitmask space_between_castlers_mask = pos_.is_w ? (is_kside ? masks::W_KSIDE_SPACE_BETWEEN_KING_AND_ROOK_MASK 
                                                                : masks::W_QSIDE_SPACE_BETWEEN_KING_AND_ROOK_MASK)
                                                    : (is_kside ? masks::B_KSIDE_SPACE_BETWEEN_KING_AND_ROOK_MASK
                                                                : masks::B_QSIDE_SPACE_BETWEEN_KING_AND_ROOK_MASK);
    
    if ((space_between_castlers_mask & pos_.occ_masks.get_occupied_squares_mask()) != 0)
        return;

    // Check that the king and rook are on the correct squares
    if (!king_and_rook_on_castle_squares(is_kside))
        return;
    
    int move_flag = is_kside ? model::Move::KING_CASTLE_FLAG 
                             : model::Move::QUEEN_CASTLE_FLAG;

    sq_t from_sq = pos_.is_w ? constants::W_KING_START_SQ : constants::B_KING_START_SQ;

    sq_t to_sq   = pos_.is_w ? (is_kside ? constants::W_KSIDE_KING_CASTLE_TO_SQ : constants::W_QSIDE_KING_CASTLE_TO_SQ)
                               : (is_kside ? constants::B_KSIDE_KING_CASTLE_TO_SQ : constants::B_QSIDE_KING_CASTLE_TO_SQ);

    // If any of the squares the king would slide through is attacked then we may not castle there
    int move_offset =   is_kside ? 1 : -1;
    if (utils::get_bit(legality_info.king_blocked_moves_mask, from_sq + move_offset) ||
        utils::get_bit(legality_info.king_blocked_moves_mask, from_sq + 2 * move_offset)) {
        return;
    }

    movelist.add_move(model::Move(from_sq, to_sq, move_flag));
}

} // namespace logic