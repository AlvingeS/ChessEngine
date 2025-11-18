#include "logic/movegen/knight_gen.h"

#include "model/position/board.h"
#include "model/move/movelist.h"

#include "logic/movegen/bitmasks/knight_bitmasks.h"
#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/bit_basics.h"

namespace logic {

KnightGenerator::KnightGenerator(model::Board& board) 
    : bitboards_(board.bitboards)
    , state_bitmasks_(board.state_bitmasks) {
    knight_masks_ = KnightBitmasks::get_all_knight_bitmasks();
}

void KnightGenerator::generate(bool is_w, model::Movelist& movelist) 
{
    std::vector<int>& knight_sq_idxs = Containers::get_piece_position_indices();
    std::vector<int>& free_moves_sq_idxs = Containers::get_leaping_piece_free_moves_indices();
    std::vector<int>& capture_moves_sq_idxs = Containers::get_leaping_piece_capturable_moves_indices();

    BitBasics::get_bit_indices(knight_sq_idxs, is_w ? bitboards_.get_w_knights_bitboard()
                                                    : bitboards_.get_b_knights_bitboard());

    for (int knight_sq_idx : knight_sq_idxs) {
        bitmask attack_mask = knight_masks_[knight_sq_idx];

        bitmask free_moves_mask = attack_mask & state_bitmasks_.get_empty_squares_bitmask();
        
        bitmask opp_pieces_mask = is_w ? state_bitmasks_.get_b_pieces_bitmask()
                                       : state_bitmasks_.get_w_pieces_bitmask();
        
        bitmask capture_moves_mask = attack_mask & opp_pieces_mask;

        BitBasics::get_bit_indices(free_moves_sq_idxs, free_moves_mask);
        BitBasics::get_bit_indices(capture_moves_sq_idxs, capture_moves_mask);

        for (int free_move_sq_idx : free_moves_sq_idxs) {
            movelist.add_move(model::Move(knight_sq_idx, free_move_sq_idx, model::Move::QUITE_FLAG));
        }

        for (int capture_move_sq_idx : capture_moves_sq_idxs) {
            movelist.add_move(model::Move(knight_sq_idx, capture_move_sq_idx, model::Move::CAPTURE_FLAG));
        }
    }
}

} // namespace logic