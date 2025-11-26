#include "logic/movegen/pawn_gen.h"

#include "model/position/board.h"
#include "model/move/movelist.h"

#include "logic/attack_tables/attack_tables.h"
#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/chess_utils.h"
#include "logic/movegen/utils/bits.h"

namespace logic {

PawnGen::PawnGen(model::Board& board) 
    : bbs_(board.bbs)
    , occupancy_masks_(board.occupancy_masks)
    , w_pawn_quiet_attack_table_(attack_tables::w_pawn_quiet)
    , w_pawn_capture_attack_table_(attack_tables::w_pawn_capture)
    , b_pawn_quiet_attack_table_(attack_tables::b_pawn_quiet)
    , b_pawn_capture_attack_table_(attack_tables::b_pawn_capture)
{}

void PawnGen::generate(
    bool is_w,
    model::Movelist& movelist,
    bitmask ep_target_mask)
{
    std::vector<sq_idx>& pawn_sqs              = Containers::get_piece_position_idxs();
    std::vector<sq_idx>& quiet_moves_idxs      = Containers::get_leaping_piece_quiet_moves_idxs();
    std::vector<sq_idx>& capture_moves_sq_idxs = Containers::get_leaping_piece_capturable_moves_idxs();

    bits::get_bit_idxs(pawn_sqs, is_w ? bbs_.get_w_pawns_bb()
                                           : bbs_.get_b_pawns_bb());

    for (int pawn_sq : pawn_sqs) {

        bitmask attack_mask_straight = is_w ? w_pawn_quiet_attack_table_[pawn_sq]
                                            : b_pawn_quiet_attack_table_[pawn_sq];

        bitmask attack_mask_diag = is_w ? w_pawn_capture_attack_table_[pawn_sq]
                                        : b_pawn_capture_attack_table_[pawn_sq];

        bitmask quiet_moves_mask = attack_mask_straight & occupancy_masks_.get_free_sqrs_mask();
        
        bitmask opp_pieces_mask = is_w ? occupancy_masks_.get_b_pieces_mask()
                                       : occupancy_masks_.get_w_pieces_mask();
        
        bitmask capture_moves_mask = attack_mask_diag & opp_pieces_mask;

        bits::get_bit_idxs(quiet_moves_idxs, quiet_moves_mask);
        bits::get_bit_idxs(capture_moves_sq_idxs, capture_moves_mask);

        int offset = is_w ? 8 : -8;
        bool can_promote = (is_w && chess_utils::rank_from_sq(pawn_sq) == 6) || (!is_w && chess_utils::rank_from_sq(pawn_sq) == 1);

        if (quiet_moves_idxs.size() == 2) {
            int single_step_idx = (is_w ? 0 : 1);
            int double_step_idx = (is_w ? 1 : 0);
            
            movelist.add_move(model::Move(pawn_sq, quiet_moves_idxs[single_step_idx], model::Move::QUITE_FLAG));
            movelist.add_move(model::Move(pawn_sq, quiet_moves_idxs[double_step_idx], model::Move::DOUBLE_PAWN_PUSH_FLAG));

        } else if (quiet_moves_idxs.size() == 1 && quiet_moves_idxs[0] == pawn_sq + offset) {
            // Only add them move it is direcly in front of the pawn, to avoid jumping over pieces
            if (can_promote) {
                movelist.add_move(model::Move(pawn_sq, quiet_moves_idxs[0], model::Move::KNIGHT_PROMO_FLAG));
                movelist.add_move(model::Move(pawn_sq, quiet_moves_idxs[0], model::Move::BISHOP_PROMO_FLAG));
                movelist.add_move(model::Move(pawn_sq, quiet_moves_idxs[0], model::Move::ROOK_PROMO_FLAG));
                movelist.add_move(model::Move(pawn_sq, quiet_moves_idxs[0], model::Move::QUEEN_PROMO_FLAG));
            
            } else {
                movelist.add_move(model::Move(pawn_sq, quiet_moves_idxs[0], model::Move::QUITE_FLAG));
            }
        }

        for (sq_idx to_sq : capture_moves_sq_idxs) {
            if (can_promote) {
                movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::QUEEN_PROMO_CAPTURE_FLAG));
                movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::ROOK_PROMO_CAPTURE_FLAG));
                movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::BISHOP_PROMO_CAPTURE_FLAG));
                movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::KNIGHT_PROMO_CAPTURE_FLAG));
            } else {
                movelist.add_move(model::Move(pawn_sq, to_sq, model::Move::CAPTURE_FLAG));
            }
        }

        if ((attack_mask_diag & ep_target_mask) != 0) {
            movelist.add_move(model::Move(pawn_sq, bits::lsb_idx(attack_mask_diag & ep_target_mask), model::Move::EP_CAPTURE_FLAG));
        }
    }
}

} // namespace logic