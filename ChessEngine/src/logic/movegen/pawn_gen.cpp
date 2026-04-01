#include "logic/movegen/pawn_gen.h"

#include "model/position/position.h"
#include "model/move/movelist.h"

#include "logic/attack_tables/attack_tables.h"
#include "logic/movegen/check_detection.h"
#include "logic/utils.h"
#include "logic/utils.h"

namespace logic {

PawnGen::PawnGen(const model::Position& pos) 
    : pos_(pos)
{}

void PawnGen::generate(model::Movelist& movelist, const LegalityInfo& legality_info)
{
    if (legality_info.in_double_check()) {
        return;
    }

    bitboard pawns_bb = pos_.is_w ? pos_.bbs.get(PieceType::W_PAWN)
                                  : pos_.bbs.get(PieceType::B_PAWN);


    utils::controlled_for_each_bit(pawns_bb, [&](sq_t pawn_sq) {
        bitmask attack_mask_straight = pos_.is_w ? attack_tables::w_pawn_quiet[pawn_sq]
                                                 : attack_tables::b_pawn_quiet[pawn_sq];

        bitmask attack_mask_diag = pos_.is_w ? attack_tables::w_pawn_capture[pawn_sq]
                                             : attack_tables::b_pawn_capture[pawn_sq];

        bitmask quiet_moves_mask = attack_mask_straight & pos_.bbs.get_empty();
        
        bitmask opp_pieces_mask = pos_.is_w ? pos_.bbs.get_b()
                                            : pos_.bbs.get_w();
        
        bitmask capture_moves_mask = attack_mask_diag & opp_pieces_mask;

        if (utils::get_bit(legality_info.pinned_mask, pawn_sq)) {
            capture_moves_mask &= legality_info.pin_rays[pawn_sq];
            quiet_moves_mask &= legality_info.pin_rays[pawn_sq];
        }

        bool can_promote = (pos_.is_w && utils::rank_from_sq(pawn_sq) == 6) ||
                           (!pos_.is_w && utils::rank_from_sq(pawn_sq) == 1);
                           
        if (legality_info.in_check()) {
            bool ep_capture_possible = (attack_mask_diag & pos_.ep_target_mask) != 0ULL;
            bitmask check_response_mask = legality_info.check_response_mask;

            // If this pawn can make an ep capture...
            if (ep_capture_possible) {
                bitmask opp_pawns = pos_.is_w ? pos_.bbs.get(PieceType::B_PAWN)
                                              : pos_.bbs.get(PieceType::W_PAWN);

                // And the checker is a pawn, then it must be that pawn that
                // created the ep target, and can thus be captured by ep
                // We extend the check response mask to also include this
                if((legality_info.checkers_mask & opp_pawns) != 0ULL) {
                    check_response_mask |= pos_.ep_target_mask;
                }
            }

            bitmask check_stopping_moves = 0ULL;
            if (ep_capture_possible) {
                check_stopping_moves = attack_mask_diag & check_response_mask;
            } else {
                check_stopping_moves = capture_moves_mask & check_response_mask;
            }
            
            // There can only be at most one legal move in check for a pawn
            if (check_stopping_moves != 0ULL) {
                sq_t capture_sq = utils::lsb_idx(check_stopping_moves);
                if (can_promote) {
                    movelist.add_move(model::Move(pawn_sq, capture_sq, model::Move::QUEEN_PROMO_CAPTURE_FLAG));
                    movelist.add_move(model::Move(pawn_sq, capture_sq, model::Move::ROOK_PROMO_CAPTURE_FLAG));
                    movelist.add_move(model::Move(pawn_sq, capture_sq, model::Move::BISHOP_PROMO_CAPTURE_FLAG));
                    movelist.add_move(model::Move(pawn_sq, capture_sq, model::Move::KNIGHT_PROMO_CAPTURE_FLAG));
                } else {
                    if (pos_.bbs.get_empty_bit(capture_sq)) {
                        movelist.add_move(model::Move(pawn_sq, capture_sq, model::Move::EP_CAPTURE_FLAG));
                    } else {
                        movelist.add_move(model::Move(pawn_sq, capture_sq, model::Move::CAPTURE_FLAG));
                    }
                }
            }

            quiet_moves_mask &= legality_info.check_response_mask;
            
            if (quiet_moves_mask != 0ULL) {
                sq_t to_sq = utils::lsb_idx(quiet_moves_mask);
                bool double_pawn_push = std::abs(pawn_sq - to_sq) == 16; 

                if (double_pawn_push) {
                    int single_step_offset = pos_.is_w ? 8 : -8;
                    bool single_step_push_free = !pos_.bbs.get_occ_bit(pawn_sq + single_step_offset);
                    if (single_step_push_free) {
                        movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::DOUBLE_PAWN_PUSH_FLAG));
                    }
                } else {
                    if (can_promote) {
                        movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::QUEEN_PROMO_FLAG));
                        movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::ROOK_PROMO_FLAG));
                        movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::BISHOP_PROMO_FLAG));
                        movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::KNIGHT_PROMO_FLAG));
                    } else {
                        movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::SINGLE_PAWN_PUSH_FLAG));
                    }
                }
            }

            return LoopControl::Continue;
        }
        
        int offset = pos_.is_w ? 8 : -8;

        if (utils::pop_count(quiet_moves_mask) == 2) {
            if (pos_.is_w) {
                movelist.add_move(model::Move(pawn_sq, utils::lsb_idx(quiet_moves_mask), model::Move::SINGLE_PAWN_PUSH_FLAG));
                movelist.add_move(model::Move(pawn_sq, utils::msb_idx(quiet_moves_mask), model::Move::DOUBLE_PAWN_PUSH_FLAG));
            } else {
                movelist.add_move(model::Move(pawn_sq, utils::msb_idx(quiet_moves_mask), model::Move::SINGLE_PAWN_PUSH_FLAG));
                movelist.add_move(model::Move(pawn_sq, utils::lsb_idx(quiet_moves_mask), model::Move::DOUBLE_PAWN_PUSH_FLAG));                
            }
        } else if (utils::pop_count(quiet_moves_mask) == 1 && utils::lsb_idx(quiet_moves_mask) == pawn_sq + offset) {
            sq_t to_sq = pawn_sq + offset;
            // Only add them move it is direcly in front of the pawn, to avoid jumping over pieces
            if (can_promote) {
                movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::KNIGHT_PROMO_FLAG));
                movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::BISHOP_PROMO_FLAG));
                movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::ROOK_PROMO_FLAG));
                movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::QUEEN_PROMO_FLAG));
            
            } else {
                movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::SINGLE_PAWN_PUSH_FLAG));
            }
        }

        utils::for_each_bit(capture_moves_mask, [&](sq_t to_sq) {
            if (can_promote) {
                movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::QUEEN_PROMO_CAPTURE_FLAG));
                movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::ROOK_PROMO_CAPTURE_FLAG));
                movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::BISHOP_PROMO_CAPTURE_FLAG));
                movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::KNIGHT_PROMO_CAPTURE_FLAG));
            } else {
                movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::CAPTURE_FLAG));
            }
        });
        
        if ((attack_mask_diag & pos_.ep_target_mask) != 0) {
            movelist.add_move(model::Move(pawn_sq, utils::lsb_idx(attack_mask_diag & pos_.ep_target_mask), model::Move::EP_CAPTURE_FLAG));
        }

        return LoopControl::Continue;
    });
}

} // namespace logic