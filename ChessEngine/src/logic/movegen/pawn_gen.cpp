#include "logic/movegen/pawn_gen.h"

#include "model/position/board.h"
#include "model/move/movelist.h"

#include "logic/attack_tables/attack_tables.h"
#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/chess_utils.h"
#include "logic/movegen/utils/bit_basics.h"

namespace logic {

PawnGenerator::PawnGenerator(model::Board& board) 
    : bitboards_(board.bitboards)
    , state_bitmasks_(board.occupancy_masks)
    , w_pawn_quiet_attack_table_(attack_tables::w_pawn_quiet)
    , w_pawn_capture_attack_table_(attack_tables::w_pawn_capture)
    , b_pawn_quiet_attack_table_(attack_tables::b_pawn_quiet)
    , b_pawn_capture_attack_table_(attack_tables::b_pawn_capture)
{}

void PawnGenerator::generate(
    bool is_w,
    model::Movelist& movelist,
    bitmask ep_target_mask)
{
    std::vector<int>& pawn_sq_idxs = Containers::get_piece_position_indices();
    std::vector<int>& free_moves_idxs = Containers::get_leaping_piece_free_moves_indices();
    std::vector<int>& capture_moves_sq_idxs = Containers::get_leaping_piece_capturable_moves_indices();

    BitBasics::get_bit_indices(pawn_sq_idxs, is_w ? bitboards_.get_w_pawns_bitboard()
                                              : bitboards_.get_b_pawns_bitboard());

    for (int pawn_sq_idx : pawn_sq_idxs) {

        bitmask attack_mask_straight = is_w ? w_pawn_quiet_attack_table_[pawn_sq_idx]
                                                  : b_pawn_quiet_attack_table_[pawn_sq_idx];

        bitmask attack_mask_diag = is_w ? w_pawn_capture_attack_table_[pawn_sq_idx]
                                                 : b_pawn_capture_attack_table_[pawn_sq_idx];

        bitmask free_moves_mask = attack_mask_straight & state_bitmasks_.get_free_squares_mask();
        
        bitmask opp_pieces_mask = is_w ? state_bitmasks_.get_b_pieces_mask()
                                      : state_bitmasks_.get_w_pieces_mask();
        
        bitmask capture_moves_mask = attack_mask_diag & opp_pieces_mask;

        BitBasics::get_bit_indices(free_moves_idxs, free_moves_mask);
        BitBasics::get_bit_indices(capture_moves_sq_idxs, capture_moves_mask);

        int offset = is_w ? 8 : -8;
        bool can_promote = (is_w && ChessUtils::rank_from_bit_index(pawn_sq_idx) == 6) || (!is_w && ChessUtils::rank_from_bit_index(pawn_sq_idx) == 1);

        if (free_moves_idxs.size() == 2) {
            int single_step_index = (is_w ? 0 : 1);
            int double_step_index = (is_w ? 1 : 0);
            
            movelist.add_move(model::Move(pawn_sq_idx, free_moves_idxs[single_step_index], model::Move::QUITE_FLAG));
            movelist.add_move(model::Move(pawn_sq_idx, free_moves_idxs[double_step_index], model::Move::DOUBLE_PAWN_PUSH_FLAG));

        } else if (free_moves_idxs.size() == 1 && free_moves_idxs[0] == pawn_sq_idx + offset) {
            // Only add them move it is direcly in front of the pawn, to avoid jumping over pieces
            if (can_promote) {
                movelist.add_move(model::Move(pawn_sq_idx, free_moves_idxs[0], model::Move::KNIGHT_PROMO_FLAG));
                movelist.add_move(model::Move(pawn_sq_idx, free_moves_idxs[0], model::Move::BISHOP_PROMO_FLAG));
                movelist.add_move(model::Move(pawn_sq_idx, free_moves_idxs[0], model::Move::ROOK_PROMO_FLAG));
                movelist.add_move(model::Move(pawn_sq_idx, free_moves_idxs[0], model::Move::QUEEN_PROMO_FLAG));
            
            } else {
                movelist.add_move(model::Move(pawn_sq_idx, free_moves_idxs[0], model::Move::QUITE_FLAG));
            }
        }

        for (int capture_move_sq_idx : capture_moves_sq_idxs) {
            if (can_promote) {
                movelist.add_move(model::Move(pawn_sq_idx, capture_move_sq_idx, model::Move::QUEEN_PROMO_CAPTURE_FLAG));
                movelist.add_move(model::Move(pawn_sq_idx, capture_move_sq_idx, model::Move::ROOK_PROMO_CAPTURE_FLAG));
                movelist.add_move(model::Move(pawn_sq_idx, capture_move_sq_idx, model::Move::BISHOP_PROMO_CAPTURE_FLAG));
                movelist.add_move(model::Move(pawn_sq_idx, capture_move_sq_idx, model::Move::KNIGHT_PROMO_CAPTURE_FLAG));
            } else {
                movelist.add_move(model::Move(pawn_sq_idx, capture_move_sq_idx, model::Move::CAPTURE_FLAG));
            }
        }

        if ((attack_mask_diag & ep_target_mask) != 0) {
            movelist.add_move(model::Move(pawn_sq_idx, BitBasics::lsb_index(attack_mask_diag & ep_target_mask), model::Move::EP_CAPTURE_FLAG));
        }
    }
}

} // namespace logic