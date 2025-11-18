#include "logic/movegen/king_gen.h"

#include "model/position/board.h"
#include "model/move/movelist.h"

#include "logic/movegen/bitmasks/king_bitmasks.h"
#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/bit_basics.h"

namespace logic {

KingGenerator::KingGenerator(model::Board& board) 
    : bitboards_(board.bitboards)
    , state_bitmasks_(board.state_bitmasks)
{
    king_bitmasks_ = KingBitmasks::get_all_king_bitmasks();
}

void KingGenerator::generate(bool is_w, model::Movelist& movelist) 
{
    std::vector<int>& king_idxs = Containers::get_piece_position_indices();
    std::vector<int>& free_moves_sq_idxs = Containers::get_leaping_piece_free_moves_indices();
    std::vector<int>& capturable_moves_sq_idxs = Containers::get_leaping_piece_capturable_moves_indices();
    
    BitBasics::get_bit_indices(king_idxs, is_w ? bitboards_.get_w_king_bitboard()
                                              : bitboards_.get_b_king_bitboard());

    int king_sq_idx = king_idxs[0];
    bitmask attack_mask = king_bitmasks_[king_sq_idx];
    bitmask free_moves_mask = attack_mask & state_bitmasks_.get_empty_squares_bitmask();
    
    bitmask enemy_pieces_mask = is_w ? state_bitmasks_.get_b_pieces_bitmask() 
                                     : state_bitmasks_.get_w_pieces_bitmask();

    bitmask capture_moves_mask = attack_mask & enemy_pieces_mask;

    BitBasics::get_bit_indices(free_moves_sq_idxs, free_moves_mask);
    BitBasics::get_bit_indices(capturable_moves_sq_idxs, capture_moves_mask);

    for (int sq_idx : free_moves_sq_idxs) {
        movelist.add_move(model::Move(king_sq_idx, sq_idx, model::Move::QUITE_FLAG));
    }

    for (int sq_idx : capturable_moves_sq_idxs) {
        movelist.add_move(model::Move(king_sq_idx, sq_idx, model::Move::CAPTURE_FLAG));
    }
}

} // namespace logic