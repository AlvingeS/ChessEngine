#include "logic/movegen/king_gen.h"

#include "model/position/board.h"
#include "model/move/movelist.h"

#include "logic/attack_tables/attack_tables.h"
#include "logic/movegen/utils/containers.h"
#include "logic/movegen/utils/bit_basics.h"

namespace logic {

KingGen::KingGen(model::Board& board) 
    : bitboards_(board.bitboards)
    , occupancy_masks_(board.occupancy_masks)
    , king_attack_table_(attack_tables::king)
{
}

void KingGen::generate(bool is_w, model::Movelist& movelist) 
{
    std::vector<int>& king_idxs = Containers::get_piece_position_indices();
    std::vector<int>& free_moves_sq_idxs = Containers::get_leaping_piece_free_moves_indices();
    std::vector<int>& capturable_moves_sq_idxs = Containers::get_leaping_piece_capturable_moves_indices();
    
    BitBasics::get_bit_indices(king_idxs, is_w ? bitboards_.get_w_king_bb()
                                              : bitboards_.get_b_king_bb());

    int king_sq_idx = king_idxs[0];
    bitmask attack_mask = king_attack_table_[king_sq_idx];
    bitmask free_moves_mask = attack_mask & occupancy_masks_.get_free_squares_mask();
    
    bitmask enemy_pieces_mask = is_w ? occupancy_masks_.get_b_pieces_mask() 
                                     : occupancy_masks_.get_w_pieces_mask();

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